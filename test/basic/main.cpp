#include <QtCore/QtCore>
#include <QtNetwork/QtNetwork>
#include <QtServer>

int main(int argc, char * argv[])
{
	QHttpApplication app(argc, argv);

	app.get("/", [](QHttpRequest & req, QHttpResponse & res) {
		res.body = "this is main page";
	});
	app.get("/hello", [](QHttpRequest & req, QHttpResponse & res) {
		res.body = "hello boy";
	});
	
	app.listen(3000);

	return app.exec();
}
