#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "libreria/XmlRpc.h"
#include "XmlRpcClient.h"

using namespace XmlRpc;

class Cliente {
private:
    XmlRpc::XmlRpcValue parametros, resultado;
    XmlRpc::XmlRpcClient* client;
    int puerto;
    const char* ip;
    int pruebaClient;
    std::string usuarioAutenticado;

public:
    Cliente(const char* _ip, int _puerto) : ip(_ip), puerto(_puerto) {}

    std::string conectarServidor() {
        client = new XmlRpcClient(ip, puerto);
        pruebaClient = client->execute("pruebaConexion", parametros, resultado);
        if (resultado)
            return "Conexión establecida \n";
        else
            return "No se ha podido establecer la conexión \n";
    }

    std::string desconectarServidor() {
        delete client;
        return "Servidor desconectado\n";
    }

    // Método para autenticar el usuario
    std::string autenticarUsuario(const std::string& nombre, const std::string& clave) {
        XmlRpcValue params, result;
        params[0] = nombre;
        params[1] = clave;

        if (client->execute("autenticar_usuario", params, result)) {
            usuarioAutenticado = nombre;  // Guardar el usuario autenticado
            return "Usuario autenticado.\n";
        } else {
            return "Usuario o contraseña incorrectos.\n";
        }
    }

    bool estaAutenticado() {
        return !usuarioAutenticado.empty();
    }

    // Modifica los métodos de acción para verificar autenticación
    std::string conectarRobot() {
        if (!estaAutenticado()) return "Usuario no autenticado.\n";
        XmlRpcValue noParams, result;
        if (client->execute("conectar_Robot", noParams, result)) {
            return "Robot conectado.\n";
        } else {
            return "Error al conectar con el robot.\n";
        }
    }

    std::string desconectarRobot() {
        if (!estaAutenticado()) return "Usuario no autenticado.\n";
        XmlRpcValue noParams, result;
        if (client->execute("desconectar_Robot", noParams, result)) {
            return "Robot desconectado.\n";
        } else {
            return "Error al desconectar el robot.\n";
        }
    }

    void activarMotores() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("activar_Motores", noParams, result)) {
            std::cout << "Motores activados." << std::endl;
        } else {
            std::cerr << "Error al activar motores." << std::endl;
        }
    }

    void desactivarMotores() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("desactivar_Motores", noParams, result)) {
            std::cout << "Motores desactivados." << std::endl;
        } else {
            std::cerr << "Error al desactivar motores." << std::endl;
        }
    }

    void moverRobot(std::string coordenadas, float velocidad) {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
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
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("activar_Efector", noParams, result)) {
            std::cout << "Efector activado." << std::endl;
        } else {
            std::cerr << "Error al activar efector." << std::endl;
        }
    }

    void desactivarEfector() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("desactivar_Efector", noParams, result)) {
            std::cout << "Efector desactivado." << std::endl;
        } else {
            std::cerr << "Error al desactivar efector." << std::endl;
        }
    }

    void homing() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("homing", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al hacer homing." << std::endl;
        }
    }

    void iniciarAprendizaje(std::string archivo) {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue params, result;
        params[0] = archivo;
        if (client->execute("iniciar_Aprendizaje", params, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al iniciar aprendizaje." << std::endl;
        }
    }

    void finalizarAprendizaje() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("finalizar_Aprendizaje", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al finalizar aprendizaje." << std::endl;
        }
    }

    void cambiarModo() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("cambiarModo", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al cambiar el modo." << std::endl;
        }
    }

    void mostrarEstado() {
        if (!estaAutenticado()) {
            std::cout << "Usuario no autenticado." << std::endl;
            return;
        }
        XmlRpcValue noParams, result;
        if (client->execute("mostrarEstado", noParams, result)) {
            std::cout << "Resultado: " << result << std::endl;
        } else {
            std::cerr << "Error al mostrar estado." << std::endl;
        }
    }
};

class InterfazUsuario {
private:
    Cliente* client;
public:
    InterfazUsuario() {
        client = new Cliente("127.0.0.1", 8080);
    }

    void interfaz() {
        int opcion;
        std::string nombre, clave;

        std::cout << "Ingrese su nombre de usuario: ";
        std::cin >> nombre;
        std::cout << "Ingrese su contraseña: ";
        std::cin >> clave;

        std::cout << client->conectarServidor();
        std::cout << client->autenticarUsuario(nombre, clave);

        do {
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
            std::cout << "13. Cambiar modo manual <----> automático\n";
            std::cout << "14. Mostrar estado\n";
            std::cout << "15. Salir\n";
            std::cout << "Seleccione una opción: ";
            std::cin >> opcion;

            switch (opcion) {
<<<<<<< HEAD
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
=======
            case 11: {
                std::string archivo;  // Declarar la variable
                std::cout << "Ingrese el nombre del archivo: "; // Solicitar al usuario que ingrese el nombre
                std::cin >> archivo;  // Asignar el valor ingresado a la variable
                client->iniciarAprendizaje(archivo); // Ahora se puede usar sin problemas
                break;
            }            
>>>>>>> fc5de6e2b9ed702460e42f4f42ddcac80f4404e0
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
=======
                case 1:
                    std::cout << client->conectarServidor();
                    break;
                case 2:
                    std::cout << client->desconectarServidor();
                    break;   
                case 3:
                    std::cout << client->conectarRobot();
                    break; 
                case 4:
                    std::cout << client->desconectarRobot();
                    break;
                case 5:
                    client->activarMotores();
                    break;
                case 6:
                    client->desactivarMotores();
                    break;
                case 7: {
                    std::string coord;
                    float velocidad;
                    std::cout << "Ingrese (x,y,z): ";
                    std::cin >> coord;
                    std::cout << "Ingrese velocidad: ";
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
                case 11: {
                    std::string archivo;
                    std::cout << "Ingrese el nombre del archivo: ";
                    std::cin >> archivo;
                    client->iniciarAprendizaje(archivo);
                    break;
                }            
                case 12:
                    client->finalizarAprendizaje();
                    break;
                case 13:
                    client->cambiarModo();
                    break;
                case 14:
                    client->mostrarEstado();
                    break;
                case 15:
                    std::cout << "Saliendo..." << std::endl;
                    break;
                default:
                    std::cout << "Opción no válida." << std::endl;
>>>>>>> 17f17b646fddc26d9c925af3ce962d1fb4622b00
            }
        } while (opcion != 15);
    }
};

int main() {
    InterfazUsuario* user_interfaz = new InterfazUsuario();
    user_interfaz->interfaz();
    return 0;
}
