#include <QtCore/QtCore>

int main(int argc, char * argv[]) {
	QCoreApplication app(argc, argv);
	QTextStream sout(stdout);
	sout << "Hello world" << endl;
	return 0;
}
