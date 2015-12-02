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
	qDebug() << "listening on port 3000";

	return app.exec();
}
