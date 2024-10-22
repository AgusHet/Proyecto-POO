#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "libreria/XmlRpc.h"
#include "XmlRpcClient.h"

using namespace XmlRpc;

class Cliente {
private:
    XmlRpc::XmlRpcValue parametros,resultado;
    XmlRpc::XmlRpcClient * client;
    int puerto;
    const char *ip;
    int pruebaClient;
public:
    Cliente(const char* _ip, int _puerto){
        ip=_ip;
        puerto=_puerto;
    }
    
    std::string conectarServidor(){
        client =new XmlRpcClient(ip,puerto);
        pruebaClient=client->execute("pruebaConexion",parametros,resultado);
        if (resultado)
            return "Conexion establecida \n";
        else
            return "No se ha podido establecer la conexion \n";
    }
    std::string desconectarServidor(){
        client->~XmlRpcClient();
        pruebaClient=0;
        return "Servidor desconectado\n";
    }


    // Conectar con el servidor para controlar el robot
    void conectarRobot() {
        XmlRpcValue noParams, result;
        if (client->execute("conectar_Robot", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al conectar con el robot." << std::endl;
        }
    }
    
    void desconectarRobot() {
        XmlRpcValue noParams, result;
        if (client->execute("desconectar_Robot", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al desconectar con el robot." << std::endl;
        }
    }

    // Activar los motores del robot
    void activarMotores() {
        XmlRpcValue noParams, result;
        if (client->execute("activar_Motores", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al activar motores." << std::endl;
        }
    }

    void desactivarMotores() {
        XmlRpcValue noParams, result;
        if (client->execute("desactivar_Motores", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al desactivar motores." << std::endl;
        }
    }


    // Mover el robot (control manual)
    void moverRobot(std::string coordenadas,float velocidad) {
        XmlRpcValue params, result;
        params[0] = coordenadas;
        params[1] = velocidad;
        
        if (client->execute("mover_Robot", params, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al mover el robot." << std::endl;
        }
    }

    void activarEfector() {
        XmlRpcValue noParams, result;
        if (client->execute("activar_Efector", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al activar efector." << std::endl;
        }
    }
    void desactivarEfector() {
        XmlRpcValue noParams, result;
        if (client->execute("desactivar_Efector", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al desactivar efector." << std::endl;
        }
    }
    void homing() {
        XmlRpcValue noParams, result;
        if (client->execute("homing", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al hacer homing." << std::endl;
        }
    }
    //Aca tiene n problema por que iniciarAprendizaje en el servidor se introdice por liena de comando 
    void iniciarAprendizaje(std::string archivo) {
        XmlRpcValue params, result;
        params[0]=archivo;
        if (client->execute("iniciar_Aprendizaje", params, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al iniciar aprendizaje." << std::endl;
        }
    }
    void finalizarAprendizaje() {
        XmlRpcValue noParams, result;
        if (client->execute("finalizar_Aprendizaje", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al finalizar aprendizaje." << std::endl;
        }
    }
    void cambiarModo() {
        XmlRpcValue noParams, result;
        if (client->execute("cambiarModo", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al cambiar el modo." << std::endl;
        }
    }
    void mostrarEstado() {
        XmlRpcValue noParams, result;
        if (client->execute("mostrarEstado", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al mostrar estado." << std::endl;
        }
    }
    /* Subir archivo G-Code
    void subirGCode(std::string archivo) {
        std::ifstream inFile(archivo);
        if (!inFile) {
            std::cerr << "Error al abrir el archivo G-Code." << std::endl;
            return;
        }

        std::string contenido((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

        XmlRpcValue params, result;
        params[0] = contenido;

        if (cliente->execute("subirGCode", params, result)) {
            std::cout << "Archivo G-Code subido con éxito." << std::endl;
        } else {
            std::cerr << "Error al subir el archivo G-Code." << std::endl;
        }
    }
*/
    // Interfaz de usuario para el cliente
    
};


class InterfazUsuario {
private:
    Cliente *client;
public:
    InterfazUsuario(){
        client=new Cliente("127.0.0.1",8080);
    }
    void interfaz() {
        int opcion;
        do {
            // Imprimir menú solo una vez por ciclo
            std::cout << "\nPanel de Control del Robot (Cliente)\n";
            std::cout << "1. Conectar al servidor\n";
            std::cout << "2. Desconectar al servidor\n";
            std::cout << "3. Conectar Robot\n";
            std::cout << "4. Desconectar Robot\n";
            std::cout << "5. Activar Motores\n";
            std::cout << "6. Desactivar Motores\n";
            std::cout << "7. Mover Robot\n";
            std::cout << "8. Activar Efector\n";
            std::cout << "9. Desactivar Efector\n";
            std::cout << "10. Homing\n";
            std::cout << "11. Activar modo aprendizaje \n";
            std::cout << "12. Finalizar aprendizaje y guardar en g code\n";
            std::cout << "13. Cambiar modo manual <----> automatico\n";
            std::cout << "14. Mostrar estado\n";
            //std::cout << "4. Subir Archivo G-Code\n";
            std::cout << "15. Salir\n";
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion) {
            case 1:
                std::cout<<client->conectarServidor();
                break;
            case 2:
                std::cout<<client->desconectarServidor();
                break;   
            case 3:
                client->conectarRobot();
                break; 
            case 4:
                client->desconectarRobot();
                break;
            case 5:
                client->activarMotores();
                break;
            case 6:
                client->desactivarMotores();
                break;

            case 7: {
                std::string coord;
                float  velocidad;
                std::cout << "Ingrese (x,y,z): ";
                std::cin >> coord;
                std::cout << "Ingrese velocidad:";
                std::cin >> velocidad;
                client->moverRobot(coord, velocidad);
                break;
            }
            case 8:
                client->activarEfector();
                break;
            
            case 9:
                client->desactivarEfector();
                break;
            case 10:
                client->homing();
                break;
<<<<<<< HEAD
            case 11:
                std::string archivo;
                client->iniciarAprendizaje(archivo);
                break;
            
=======
            case 11: {
                std::string archivo;  // Declarar la variable
                std::cout << "Ingrese el nombre del archivo: "; // Solicitar al usuario que ingrese el nombre
                std::cin >> archivo;  // Asignar el valor ingresado a la variable
                client->iniciarAprendizaje(archivo); // Ahora se puede usar sin problemas
                break;
            }            
>>>>>>> 7eaa81c34a213cef0a9ed673898571ff88b67be9
            case 12:
                client->finalizarAprendizaje();
                break;
            case 13:
                client->cambiarModo();
                break;
            case 14:
                client->mostrarEstado();
                break;
            /*case 4: {
                std::string archivo;
                std::cout << "Ingrese el nombre del archivo G-Code: ";
                std::cin >> archivo;
                subirGCode(archivo);
                break;
            }
            */
            case 15:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida." << std::endl;
            }
        } while (opcion != 15);
    }


};

int main() {
    InterfazUsuario *user_interfaz;
    user_interfaz=new InterfazUsuario();
    user_interfaz->interfaz();
    
    
    return 0;
}
