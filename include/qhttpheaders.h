#ifndef QHTTPHEADERS_H
#define QHTTPHEADERS_H

#include "qtserverglobal.h"
#include <QtCore/QHash>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class Q_SERVER_EXPORT QHttpHeaders : public QHash<QString, QString>
{
public:
	typedef QHash<QString, QString> super_type;
	typedef super_type::const_iterator const_iterator;
	typedef super_type::iterator iterator;
	typedef QString T;
	typedef QString Key;
public:
	inline const_iterator constFind(const Key & key) const
	{ return super_type::constFind(key.toLower()); }

	inline bool contains(const Key & key) const
	{ return super_type::contains(key.toLower()); }

	inline int count(const Key & key) const
	{ return super_type::count(key.toLower()); }

	inline iterator find(const Key & key)
	{ return super_type::find(key.toLower()); }

	inline const_iterator find(const Key & key) const
	{ return super_type::find(key.toLower()); }

	inline iterator insert(const Key & key, const T & value)
	{ return super_type::insert(key.toLower(), value); }

	inline iterator insertMulti(const Key & key, const T & value)
	{ return super_type::insertMulti(key.toLower(), value); }

	inline int remove(const Key & key)
	{ return super_type::remove(key.toLower()); }

	inline T take(const Key & key)
	{ return super_type::take(key.toLower()); }

	inline const T value(const Key & key) const
	{ return super_type::value(key.toLower()); }

	inline const T value(const Key & key, const T & defaultValue) const
	{ return super_type::value(key.toLower(), defaultValue); }

	inline QList<T> values(const Key & key) const
	{ return super_type::values(key.toLower()); }

	inline T & operator[](const Key & key)
	{ return super_type::operator[](key.toLower()); }

	inline const T operator[](const Key & key) const
	{ return super_type::operator[](key.toLower()); }
};

QT_END_NAMESPACE

#endif
