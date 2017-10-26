// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef IRANBIT_QT_IRANBITADDRESSVALIDATOR_H
#define IRANBIT_QT_IRANBITADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class IranbitAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit IranbitAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Iranbit address widget validator, checks for a valid iranbit address.
 */
class IranbitAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit IranbitAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // IRANBIT_QT_IRANBITADDRESSVALIDATOR_H
