#ifndef CONSISTENCY_CHECK_H
#define CONSISTENCY_CHECK_H
#include <QMainWindow>
bool check_consistency(QVector<QString> xml);
QMap<qint32, QString> identify_errors(QVector<QString> xml);
#endif // CONSISTENCY_CHECK_H
