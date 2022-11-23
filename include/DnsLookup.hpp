#ifndef QT_EXAMPLE_DNS_LOOKUP_HPP
#define QT_EXAMPLE_DNS_LOOKUP_HPP

#include <QDnsLookup>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWidget>

class DnsLookup : public QWidget {
  Q_OBJECT

 public:
  explicit DnsLookup(QString dnsServer);

 private:
  QVBoxLayout* m_layout;
  QGridLayout* m_inputLayout;
  QTableWidget* m_resultTable;
  QLineEdit* m_url;
  QPushButton* m_send;

  QDnsLookup* m_dns;

  QString m_dnsServer;

  void addResult(QString type, int ttl, QString result);
  bool validHostname() const;

 private Q_SLOTS:
  void send();
  void lookupFinished();
  void validate();
};

#endif