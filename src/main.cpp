#include <DnsLookup.hpp>
#include <QApplication>
#include <QLabel>
#include <QStyleFactory>
#include <QTranslator>

int main(int argc, char* argv[]) {
#ifdef _WIN32
  QApplication::setStyle(QStyleFactory::create("fusion"));
#endif

  QApplication app(argc, argv);

  DnsLookup widget("8.8.8.8");
  widget.setWindowTitle("DNS tools");
  widget.setWindowIcon(QIcon(":/images/dns.ico"));
  widget.resize(854, 480);
  widget.setMinimumSize(500, 300);
  widget.show();

  return app.exec();
}
