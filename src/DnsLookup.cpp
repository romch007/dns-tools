#include <DnsLookup.hpp>
#include <QHeaderView>
#include <QHostAddress>
#include <QLabel>
#include <QMessageBox>
#include <QRegularExpression>

DnsLookup::DnsLookup(QString dnsServer)
    : QWidget(), m_dnsServer(std::move(dnsServer)) {
  m_layout = new QVBoxLayout;
  m_inputLayout = new QGridLayout;
  m_url = new QLineEdit;
  m_send = new QPushButton(tr("&Send"));
  m_dns = new QDnsLookup(this);

  m_resultTable = new QTableWidget;

  connect(m_send, &QPushButton::released, this, &DnsLookup::send);
  connect(m_url, &QLineEdit::returnPressed, this, &DnsLookup::send);
  connect(m_dns, &QDnsLookup::finished, this, &DnsLookup::lookupFinished);
  connect(m_url, &QLineEdit::textChanged, this, &DnsLookup::validate);

  m_url->setPlaceholderText(tr("Hostname"));

  m_resultTable->setColumnCount(3);
  m_resultTable->setHorizontalHeaderLabels({tr("Type"), "TTL", tr("Result")});
  m_resultTable->horizontalHeader()->setStretchLastSection(true);
  m_resultTable->verticalHeader()->hide();

  m_inputLayout->addWidget(m_url, 0, 0, 1, 4);
  m_inputLayout->addWidget(m_send, 0, 5, 1, 1);

  m_layout->addLayout(m_inputLayout);
  m_layout->addWidget(m_resultTable);
  m_layout->setAlignment(Qt::AlignTop);

  setLayout(m_layout);

  validate();
}

void DnsLookup::send() {
  if (!validHostname())
    return;
  m_resultTable->setRowCount(0);
  m_dns->setType(QDnsLookup::ANY);
  m_dns->setNameserver(QHostAddress(m_dnsServer));
  m_dns->setName(m_url->text());
  m_dns->lookup();
  m_send->setText(tr("Loading") + "...");
  m_send->setEnabled(false);
}

void DnsLookup::lookupFinished() {
  m_send->setText("Send");
  m_send->setEnabled(true);
  if (m_dns->error() != QDnsLookup::NoError) {
    QMessageBox::critical(this, tr("Error"), tr("DNS Lookup failed"));
    return;
  }

  // A and AAAA
  const auto hostRecords = m_dns->hostAddressRecords();
  for (const auto& record : hostRecords) {
    addResult(record.value().protocol() ==
                      QHostAddress::NetworkLayerProtocol::IPv4Protocol
                  ? "A"
                  : "AAAA",
              record.timeToLive(), record.value().toString());
  }

  // CNAME
  const auto cnameRecords = m_dns->canonicalNameRecords();
  for (const auto& record : cnameRecords) {
    addResult("CNAME", record.timeToLive(), record.value());
  }

  // MX
  const auto mxRecords = m_dns->mailExchangeRecords();
  for (const auto& record : mxRecords) {
    addResult("MX", record.timeToLive(), record.name());
  }

  // PTR
  const auto ptrRecords = m_dns->pointerRecords();
  for (const auto& record : ptrRecords) {
    addResult("PTR", record.timeToLive(), record.value());
  }
}

void DnsLookup::addResult(const QString& type, int ttl, const QString& result) {
  int row = m_resultTable->rowCount();
  m_resultTable->insertRow(row);
  m_resultTable->setItem(row, 0, new QTableWidgetItem(type));
  m_resultTable->setItem(row, 1, new QTableWidgetItem(QString::number(ttl)));
  m_resultTable->setItem(row, 2, new QTableWidgetItem(result));
}

bool DnsLookup::validHostname() const {
  static QRegularExpression hostnameRegexp(
      R"(^(([a-zA-Z0-9]|[a-zA-Z0-9][a-zA-Z0-9\-]*[a-zA-Z0-9])\.)*([A-Za-z0-9]|[A-Za-z0-9][A-Za-z0-9\-]*[A-Za-z0-9])$)");

  QRegularExpressionMatch match = hostnameRegexp.match(m_url->text());

  return match.hasMatch();
}

void DnsLookup::validate() {
  m_send->setDisabled(!validHostname());
}
