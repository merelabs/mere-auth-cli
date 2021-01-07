#include "cli.h"
#include "mere/auth/service.h"

#include <QCommandLineParser>

Cli::Cli(QObject *parent) : QObject(parent)
{

}

void Cli::run()
{
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption usernameOption(QStringList() << "u" << "username",
                                      QCoreApplication::translate("main", "Set the username"),
                                      "username");

    QCommandLineOption passwordOption(QStringList() << "p" << "password",
                                      QCoreApplication::translate("main", "Set the username"),
                                      "password");

    QCommandLineOption userListOption(QStringList() << "l" << "list",
                                      QCoreApplication::translate("main", "List all of the users"));

    parser.addOption(usernameOption);
    parser.addOption(passwordOption);
    parser.addOption(userListOption);

    parser.process(QCoreApplication::arguments());

    if(parser.isSet(userListOption))
    {
        Mere::Auth::Service service;

        for (const auto &user : service.users(Mere::Auth::User::Type::SuperUser | Mere::Auth::User::Type::NormalUser))
        {
            qDebug() << user.name() << user.profile().home() << user.profile().shell();
        }
    }
    else
    {
        QString username = parser.value(usernameOption);
        QString password = parser.value(passwordOption);

        Mere::Auth::Service service;
        bool ok = service.login(username, password);
        if (ok)
        {
            qDebug() << "Yes, a valid user of this system.";

            try
            {
                Mere::Auth::User user = service.user(username);
                qDebug() << "Username:" << user.name();
                qDebug() << "Uid:" << user.uid();
                qDebug() << "Gid:" << user.gid();
                qDebug() << "Name:" << user.profile().name();

                Mere::Auth::UserProfile profile = user.profile();
                for (const auto &group : profile.groups())
                {
                    qDebug() << "Gid:" << group.gid();
                    qDebug() << "Name:" << group.name();
                    for (const auto &member : group.members())
                    {
                        qDebug() << "Member:" << member;
                    }
                }
            }
            catch(std::exception &ex)
            {
                qDebug() << ex.what();
            }
        }
        else
        {
            qDebug() << "Sorry, not a valid user of this system.";
            ::exit(1);
        }
    }

    ::exit(0);
}
