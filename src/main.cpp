/*
    SPDX-FileCopyrightText: 2008 Ian Wadham <iandw.au@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include <QApplication>
#include <QCommandLineParser>

#include <KAboutData>
#include <KCrash>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <Kdelibs4ConfigMigrator>
#endif
#include <KLocalizedString>

#include "kubrick.h"
#include "kubrick_version.h"

int main(int argc, char **argv)
{
    // Fixes blurry icons with fractional scaling
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
    QApplication app(argc, argv);

    KLocalizedString::setApplicationDomain("kubrick");

    KAboutData about (QStringLiteral("kubrick"), i18n ("Kubrick"),
            KUBRICK_VERSION_STRING, i18n ("A game based on Rubik's Cube (TM)"),
            KAboutLicense::GPL,
            i18n ("&copy; 2008 Ian Wadham"),
            QString(),
            QStringLiteral("https://apps.kde.org/kubrick") );
    about.addAuthor (i18n ("Ian Wadham"), i18n ("Author"),
                             QStringLiteral("iandw.au@gmail.com"));
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Kdelibs4ConfigMigrator migrate(QStringLiteral("kubrick"));
    migrate.setConfigFiles(QStringList() << QStringLiteral("kubrickrc"));
    migrate.setUiFiles(QStringList() << QStringLiteral("kubrickui.rc"));
    migrate.migrate();
#endif

    QCommandLineParser parser;
    KAboutData::setApplicationData(about);
    KCrash::initialize();
    about.setupCommandLine(&parser);
    parser.process(app);
    about.processCommandLine(&parser);

    Kubrick * mainWindow = nullptr;

    if (app.isSessionRestored ()) {
        kRestoreMainWindows<Kubrick>();
    }
    else {
        // No interrupted session ... just start up normally.

        /// @todo do something with the command line args here

        mainWindow = new Kubrick ();
        mainWindow->show ();
    }

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("kubrick")));

    // MainWindow has WDestructiveClose flag by default, so
    // it will delete itself.
    return app.exec ();
}
