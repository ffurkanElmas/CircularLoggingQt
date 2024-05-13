#pragma warning(disable : 4996)
#include <QCoreApplication>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <thread>
#include <chrono>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

using namespace std;

class Logger {

public:
    QString logType;
    string logFilesLocation;
    int creationRate;
    int	maxFiles;
    int numberOfFiles;
    ofstream logFile;
    vector<string> fileList;
    string jsonFileLocation;
    bool run;


public:
    Logger(const string& jsonAddress, const string& logsAdress) : jsonFileLocation(jsonAddress), logFilesLocation(logsAdress) {

        numberOfFiles=0;
        run = true;

        QFile file(jsonFileLocation.c_str());
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Dosya Acilamadi";
            return;
        }


        QString fileContent = file.readAll();
        file.close();

        QJsonDocument document = QJsonDocument::fromJson(fileContent.toUtf8());
        QJsonObject rootObject = document.object();

        maxFiles = rootObject["max_files"].toInt();
        creationRate = rootObject["creation_rate"].toInt();
        logType = rootObject["log_type"].toString();

    }


public:
    void log(string logMessage) {

        while (run) {

            string logFileName = "C:/Dosyalar/Logs/" + dateNow() + ".log";

            logFile.open(logFileName, ios::app);
            logFile << dateNow() + " : " + logMessage;
            logFile.close();

            numberOfFiles++;

            fileList.push_back(dateNow() + ".log");
            cout << "Logged." << endl;
            cout << fileList.back() << endl;

            if (logType == "second") std::this_thread::sleep_for(std::chrono::seconds(creationRate));
            else if (logType == "minute") std::this_thread::sleep_for(std::chrono::minutes(creationRate));
            else if (logType == "hour") std::this_thread::sleep_for(std::chrono::hours(creationRate));
            else if (logType == "day") std::this_thread::sleep_for(std::chrono::hours(creationRate * 24));

            manageLogFiles();

        }
    }


public:
    string dateNow() {

        time_t dateNow = time(NULL);
        struct tm* dateNowInfo = localtime(&dateNow);
        char date[80];
        strftime(date, sizeof(date), "%Y-%m-%d %H_%M_%S", dateNowInfo);

        return string(date);
    }


public:
    void manageLogFiles() {

        while (numberOfFiles >= maxFiles) {

            string oldestFile = logFilesLocation + fileList.front();
            std::filesystem::remove(oldestFile.c_str());
            fileList.erase(fileList.begin());
            numberOfFiles--;
            cout << numberOfFiles;
        }

    }

};

int main() {

    Logger log("C:/Dosyalar/Logs/config.json", "C:/Dosyalar/Logs/");    //Programin calismasi icin buraya JSON dosyasinin konumu ve log dosyalarinin olusturulacagi konum yazilmalidır.
    log.log("Deneme mesaji");



}
