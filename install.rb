#!/usr/bin/env ruby

require 'fileutils'

QT_VERSION = 'v5.5.1'
QT_GIT = 'https://github.com/qtproject/qtbase.git'

def cpu_number
  case RbConfig::CONFIG['host_os']
  when /darwin9/
    `hwprefs cpu_count`.to_i
  when /darwin/
    ((`which hwprefs` != '') ? `hwprefs thread_count` : `sysctl -n hw.ncpu`).to_i
  when /linux/
    `cat /proc/cpuinfo | grep processor | wc -l`.to_i
  when /freebsd/
    `sysctl -n hw.ncpu`.to_i
  when /mswin|mingw/
    require 'win32ole'
    wmi = WIN32OLE.connect("winmgmts://")
    cpu = wmi.ExecQuery("select NumberOfCores from Win32_Processor") # TODO count hyper-threaded in this
    cpu.to_enum.first.NumberOfCores
  end
end

def dep_find x
  ENV['PATH'].split(':').each do |p|
    if File.exists? p + '/' + x
      return p + '/' + x
    end
  end
  throw "#{x} is not found in path"
end

def dep_check
  dep_find 'git'
  dep_find 'make'
  dep_find 'gcc'
  dep_find 'g++'
  dep_find 'cmake'
end

def run_shell cmd
  puts "$ #{cmd}"
  if !system(cmd)
    throw 'error from shell'
  end
end

def checkout_qtbase
  if !File.exists? 'qtbase/.git/config'
    puts 'Cloning Qt Base...'
    run_shell("git clone #{QT_GIT}")
  end

  ENV['GIT_DIR'] = Dir.pwd + '/qtbase/.git'
  if %x(git describe).strip != QT_VERSION
    run_shell("cd qtbase && git checkout #{QT_VERSION}")
  end
end

def configure_qtbase
  if !File.exists? 'qtbase/Makefile'
    puts 'Configuring Qt Base...'
    arg = "-opensource -confirm-license -release -static -no-gui -no-xcb -no-icu -qt-pcre"
    run_shell("cd qtbase && ./configure #{arg} -j #{cpu_number}")
  end
end

def build_qtbase
  if !File.exists? 'qtbase/lib/libQt5Network.a'
    puts 'Building Qt Base...'
    run_shell("cd qtbase && make sub-src-qmake_all")
    run_shell("cd qtbase/src && make sub-corelib sub-network")
  end
end

def make_qtserver
  if !File.exists? 'build/src/libQtServer.a'
    FileUtils.mkdir_p 'build'
    run_shell("cd build && cmake .. && make -j #{cpu_number}")
  end
end

def install_qsm
  qsm = Dir.home + '/bin/qsm'
  qsm_this = Dir.pwd + '/qsm/bin/qsm'
  if File.exists? qsm
    if File.readlink(qsm) == qsm_this
      puts 'qsm is installed'
    else
      puts 'Another version of QtServer is installed.'
      puts 'If you want to change your QtServer, remove $HOME/bin/qsm'
      puts 'and create a new symbol link to ' + qsm_this
    end
  else
    File.symlink(qsm, qsm_this)
  end
end


dep_check
checkout_qtbase
configure_qtbase
build_qtbase
install_qsm
