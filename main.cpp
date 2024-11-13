#include <iostream>
#include <fstream>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <chrono>
#include <ctime>
#include <iomanip> // Para std::put_time

class Bitacora {
public:
    // Método estático para agregar una entrada a la bitácora con la fecha del sistema
    static void agregarEntrada(const std::string& detalle) {
        // Obtener la fecha y hora actual del sistema
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* now_tm = std::localtime(&now_time);

        // Formatear la fecha en un formato legible (YYYY-MM-DD HH:MM:SS)
        std::ostringstream fechaStream;
        fechaStream << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");
        std::string fecha = fechaStream.str();

        // Cargar el archivo JSON existente (si existe)
        QJsonArray bitacora;
        QFile archivoEntrada("bitacora.json");
        if (archivoEntrada.open(QIODevice::ReadOnly)) {
            QByteArray fileData = archivoEntrada.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(fileData);
            if (doc.isArray()) {
                bitacora = doc.array();
            }
            archivoEntrada.close();
        }

        // Agregar una nueva entrada a la bitácora
        QJsonObject nuevaEntrada;
        nuevaEntrada["fecha"] = QString::fromStdString(fecha);
        nuevaEntrada["detalle"] = QString::fromStdString(detalle);

        bitacora.append(nuevaEntrada);

        // Guardar la bitácora actualizada en el archivo
        QFile archivoSalida("bitacora.json");
        if (archivoSalida.open(QIODevice::WriteOnly)) {
            QJsonDocument doc(bitacora);
            archivoSalida.write(doc.toJson(QJsonDocument::Indented));
            archivoSalida.close();
        } else {
            std::cerr << "Error al abrir el archivo para escritura" << std::endl;
        }
    }

    // Método estático para mostrar la bitácora completa
    static void mostrarBitacora() {
        QJsonArray bitacora;
        QFile archivoEntrada("log.json");
        if (archivoEntrada.open(QIODevice::ReadOnly)) {
            QByteArray fileData = archivoEntrada.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(fileData);
            if (doc.isArray()) {
                bitacora = doc.array();
            }
            archivoEntrada.close();

            for (const auto& entrada : bitacora) {
                QJsonObject obj = entrada.toObject();
                std::cout << "Fecha: " << obj["fecha"].toString().toStdString()
                          << ", Detalle: " << obj["detalle"].toString().toStdString()
                          << std::endl;
            }
        } else {
            std::cerr << "error" << std::endl;
        }
    }
};

int main() {
    Bitacora::agregarEntrada("Creación de la bitácora");


    std::cout << "Contenido de la bitácora:" << std::endl;
    Bitacora::mostrarBitacora();

    return 0;
}
