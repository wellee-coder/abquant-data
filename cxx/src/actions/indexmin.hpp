/****************************************************************************
 * Copyright (c) 2020-2026 Jimmy M. Gong                                    *
 * All rights reserved.                                                     *
 *                                                                          *
 * Distributed under the terms of the GPL3 and LGPL3 Licenses.              *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#pragma once

#include <QDebug>
#include <algorithm>
#include <experimental/propagate_const>
#include <iostream>

#include "abquant/actions/index.hpp"
#include "abquant/actions/utils.hpp"
#include "abquant/models/indexmin.h"

namespace abq
{
class IndexMinAction : public IndexAction<IndexMinAction>, std::enable_shared_from_this<IndexMinAction>
{
public:
    //! Default constructor
    IndexMinAction() = default;

    IndexMinAction(QStringList codes, const char* start, const char* end, MIN_FREQ freq = MIN_FREQ::ONE);

    //! Copy constructor
    IndexMinAction(const IndexMinAction& other) = delete;

    //! Move constructor
    IndexMinAction(IndexMinAction&& other) noexcept = delete;

    //! Destructor
    ~IndexMinAction();

    //! Copy assignment operator
    IndexMinAction& operator=(const IndexMinAction& other) = default;

    //! Move assignment operator
    IndexMinAction& operator=(IndexMinAction&& other) noexcept;

    QList<IndexMin> getIndexes() const;
    QVector<const char*> getColumns() const;
    MyDataFramePtr toFq(FQ_TYPE fq = FQ_TYPE::NONE);
    MyDataFramePtr getDataFrame() const;
    QStringList getCodes() const;

    template <typename T>
    QVector<T> toSeries(const char*) const noexcept;

private:
    class impl;
    std::experimental::propagate_const<std::shared_ptr<impl>> pImpl;
    FQ_TYPE m_xdxr{FQ_TYPE::NONE};

private:
    friend inline QDebug operator<<(QDebug d, const IndexMinAction& sa)
    {
        QVector<const char*> columns = sa.getColumns();
        d << columns << "\n";
        auto qs = sa.getIndexes();
        d << qs.size() << "\n";

        QVector<QList<QVariant>> qv;
        for (auto s : qs) {
            QList<QVariant> valuelist;
            valuelist << QVariant(s.open()) << QVariant(s.close()) << QVariant(s.high()) << QVariant(s.low())
                      << QVariant(s.vol()) << QVariant(s.amount()) << QVariant(s.date()) << QVariant(s.code())
                      << QVariant(s.dateStamp());
            d << valuelist;
            qv << valuelist;
            d << QVariant("\n");
        }
        return d;
    }
};

template <typename T>
QVector<T> IndexMinAction::toSeries(const char* col) const noexcept
{
    QVector<T> series;
    auto cols = getColumns();
    if (std::none_of(cols.cbegin(), cols.cend(), [col](const char* c) { return QString(c) == QString(col); })) {
        return series;
    }
    for (auto s : getIndexes()) {
        if constexpr (std::is_same_v<T, double>) {
            if (QString("open") == QString(col)) {
                series << s.open();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("close") == QString(col)) {
                series << s.close();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("high") == QString(col)) {
                series << s.high();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("low") == QString(col)) {
                series << s.low();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("vol") == QString(col)) {
                series << s.vol();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("amount") == QString(col)) {
                series << s.amount();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("datetime") == QString(col)) {
                series << s.datetime().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("code") == QString(col)) {
                series << s.code().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("date") == QString(col)) {
                series << s.date().toStdString();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("date_stamp") == QString(col)) {
                series << s.dateStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, double>) {
            if (QString("time_stamp") == QString(col)) {
                series << s.timeStamp();
                continue;
            }
        }
        if constexpr (std::is_same_v<T, std::string>) {
            if (QString("type") == QString(col)) {
                series << s.type().toStdString();
                continue;
            }
        }
    }
    return series;
}

} // namespace abq
