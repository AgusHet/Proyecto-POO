import xmlrpc.server
from xmlrpc.server import SimpleXMLRPCServer
import time
import json
import pickle
import threading
import os 
## Falta corregir: Para que cuando se pone en mdodo automatico permita seleccionar un G-code a aplicar 
#Tambien que iniciar aprendizaje requiere introducir por teclado el nombre, pero cuando el cliente lo quiere usar no puede 
#Corregir que en el logger siempre lo guarda como local host aun que sea el cliente hay que cambiar eso 
#Ver si es necesario que robot tenga el puerto serie y que se haga la transmision hacia el arduino 

# Clase para el método ConectarRobot
class Robot:
    
    def __init__(self ,logger,manejo_Gcode):
        #self.puertoSerial=puertoserial
        #self.Baudios=Baudios
        self.conexion=False
        self.estado_motores=False
        self.logger=logger
        self.posicion=(0,0,0)  #Es una tupla 
        self.modoTrabajo= "manual"
        self.efector="Desactivado"
        self.aprender=False
        self.manejo_Gcode=manejo_Gcode


    def conectar(self):
        self.conexion=True
        self.logger.guardar_log("Conectando al robot", "localhost", "admin", True)
        return "Robot conectado con éxito."
    
    
    def desconectar(self):
        self.conexion=False
        self.logger.guardar_log("Desconectando al robot", "localhost", "admin", True)
        return "Robot desconectado con éxito."



    def activar_Motores(self):
           
        if self.conexion:
            self.estado_motores= True
            self.logger.guardar_log("Activando motores", "localhost", "admin", True)
            
            gcode_instruccion = "M17"

            if self.aprender:
                self.manejo_Gcode.escribir_Instruccion(gcode_instruccion)
            return "Motores activados."
        else:
            return "Error: El robot no está conectado. Conéctelo antes de cambiar el estado de los motores."


    def desactivar_Motores(self):
           
        if self.conexion:
            self.estado_motores= False
            self.logger.guardar_log("Desactivado motores", "localhost", "admin", True)
            
            gcode_instruccion = "M18"
            if self.aprender:
                self.manejo_Gcode.escribir_Instruccion(gcode_instruccion)

            return "Motores desactivados."
        else:
            return "Error: El robot no está conectado. Conéctelo antes de cambiar el estado de los motores."

    def mover(self,posicion, velocidad):
        if self.conexion and self.estado_motores:
            try:
                pos_tupla=tuple(map(float,posicion.strip("()").split(",")))  #Transformamos en una tupla la posicion ingresada 
                velocidad=float(velocidad)
            except ValueError:
                print("Entrada invalida para posicion o velocidad.")
            self.posicion=pos_tupla
            self.logger.guardar_log (f"Movimiento a {posicion} con velocidad {velocidad}", "localhost", "admin", True)
            
            instruccion_Gcode=f"G1 x{self.posicion[0]} Y {self.posicion[1]} Z {self.posicion[0]} F{velocidad}" #Ver si hay alguna forma especifica de ponerlo en g code 
            if self.aprender:
                
                self.manejo_Gcode.escribir_Instruccion(instruccion_Gcode)
        else :
            return "Error: Robot no conectado o motores apagados."

    def activar_Efector(self):
        if self.conexion and self.estado_motores    :
            self.efector="Activado"
            self.logger.guardar_log("Efector activado", "localhost", "admin", True)
            if self.aprender:
                instruccion_Gcode="M3;Activar efector"
                self.manejo_Gcode.escribir_Instruccion(instruccion_Gcode)
        else:
            return "Error: Robot no conectado o motores apagados."
    
    def desactivar_Efector(self):
        if self.conexion and self.estado_motores:
            self.efector="Desactivado"
            self.logger.guardar_log("Efector desactivado", "localhost", "admin", True)
            if self.aprender:
                
                gcode_instruccion = "M5 ; Desactivar efector"
                self.manejo_Gcode.escribir_Instruccion(gcode_instruccion)
        else:
            return "Error: Robot no conectado o motores apagados."

    def homing(self):
        if self.conexion and self.estado_motores:
            self.posicion=(0,0,0)
            self.desactivar_Efector()
            self.logger.guardar_log(" Homing", "localhost", "admin", True)

            gcode_instruccion = "G28"
            if self.aprender:
                # Instrucción para homing en G-code
                
                self.manejo_Gcode.escribir_Instruccion(gcode_instruccion)
            return "Homing realizado"

        else:
            return "Error: Robot no conectado o motores apagados."
        
    def iniciar_Aprendizaje(self):
        if self.conexion:
            self.aprender=True
            self.nombre=input("Introduzca el nombre con el que quiere guardar el archivo G-code: ")
            self.manejo_Gcode.iniciar_Nuevo_Archivo(self.nombre)
            print("Modo aprendizaje activado")
        else:
            return "Error: El robot no está conectado."

    def finalizar_Aprendizaje(self):
        if self.conexion:
            self.aprender=False
            self.manejo_Gcode.cerrar_Archivo()
            print("Modo de aprendizaje finalizado, archivo guardado.")
        else:
            return "Error: El robot no está conectado."


    def cambiar_Modo(self):
        if self.modoTrabajo == "manual":
            self.modoTrabajo = "automático"
        else:
            self.modoTrabajo = "manual"
        self.logger.guardar_log(f"Modo cambiado a {self.modoTrabajo}", "localhost", "admin", True)
        return f"Modo cambiado a {self.modoTrabajo}"

    def mostrar_Estado(self):
        gcode_instruccion = "G114"
        if self.aprender:
            
            
            self.manejo_Gcode.escribir_Instruccion(gcode_instruccion)
        return {
            "Estado de conexión": self.conexion,
            "Estado de motores": self.estado_motores,
            "Posición actual": self.posicion,
            "Modo de trabajo": self.modoTrabajo
        }


class ManejoGcode:
    def __init__(self, directory="gcode_files"):
        self.directory=directory
        if not os.path.exists(self.directory):
            os.makedirs(self.directory)
        self.archivo=None
        
    
    def iniciar_Nuevo_Archivo(self, nombre_Gcode):
        self.nombre_Gcode=nombre_Gcode
        self.archivo=open(os.path.join(self.directory,nombre_Gcode),"w")
        print(f"Nuevo archivo G-code iniciado:{nombre_Gcode}")
    
    def escribir_Instruccion(self, instruccion):
        if self.archivo:
            self.archivo.write(instruccion + "\n")


    def cerrar_Archivo(self):
        if self.archivo:
            self.archivo.close()
            print("Archivo G-code guardado con exito.")

    def leer_Archivo(self,nombre_Gcode):
        with open(os.path.join(self.directory,nombre_Gcode),"r")as archivo:
            return archivo.read()
    


# Clase para el manejo de usuarios
class Usuario:
    def __init__(self, nombre, clave):
        self.nombre = nombre
        self.clave = clave

class ManejoUsuarios:
    def __init__(self, archivo_usuarios="usuarios.pkl"):
        self.archivo_usuarios = archivo_usuarios
        try:
            with open(self.archivo_usuarios, "rb") as file:
                self.usuarios = pickle.load(file)
        except FileNotFoundError:
            self.usuarios = {}

    def validar_usuario(self, nombre, clave):
        if nombre in self.usuarios and self.usuarios[nombre] == clave:
            return True
        return False

    def agregar_usuario(self, nombre, clave):
        self.usuarios[nombre] = clave
        with open(self.archivo_usuarios, "wb") as file:
            pickle.dump(self.usuarios, file)

# Clase Logger para guardar registros
class Logger:
    def __init__(self, log_file="log_trabajo.csv"):
        self.log_file = log_file

    def guardar_log(self, mensaje, ip, usuario, exito):
        with open(self.log_file, "a") as file:
            now = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            resultado = "Exitoso" if exito else "Fallido"
            file.write(f"{now}, {ip}, {usuario}, {mensaje}, {resultado}\n")
        

    def mostrar_ultimas_lineas(self, n=100):
        with open(self.log_file, "r") as file:
            lineas = file.readlines()[-n:]
        return ''.join(lineas)


# Clase para listar comandos disponibles y ofrecer ayuda
class Ayuda:
    def __init__(self):
        self.comandos = {
            "conectar / desconectar": "Conectar o desconectar al robot.",
            "activar_Motores / desactivar_Motores": "Activar o desactivar los motores del robot.",
            "cambiar_Modo": "Cambiar el modo del robot entre manual y automático.",
            "mover": "Mover el robot a una posición específica con una determinada velocidad. Ej posicion (3,2,5)  velocidad 2 ",
            "activar_Efector / desactivar_Efector": "Cerrar o desactivar el efector ",
            "homing":"Mueve el robot a una posicion de origen correspodiente a (0,0,0)",
            "mostrar_Estado" : "Muestra el estado del robot",
            "iniciar_Aprendizaje / finalizar_Aprendizaje":"Permite enseñar una trayectoria y guardarla en un archivo G-code",
            "cargarGCode": "Cargar un archivo G-Code para ejecutar.",
        }

    def listar_comandos(self):
        return self.comandos

    def mostrar_ayuda(self, comando):
        if comando in self.comandos:
            return f"Comando: {comando} - {self.comandos[comando]}"
        else:
            return "Comando no reconocido."



# Menú del servidor 
class PanelControl:
    def __init__(self):    #def __init__(self, servidor,robot): esta es una opcion pero creo que servidor no hace falta
        self.logger=Logger(log_file="log_trabajo.csv")    #El panel de control se incia pasandole una instancia de robot, que deberia ser la misma con la que trabaja el servidor 
        self.manejo_Gcode=ManejoGcode()  #Instancia de manejo de g code 
        self.robot=Robot(logger=self.logger,manejo_Gcode=self.manejo_Gcode) #instancia de robot 
        self.server =None
        self.ayuda = Ayuda()

    def iniciar_Servidor(self):
        if self.server is None:
            self.server=ServidorRPCCompleto(self.robot,self.logger) #aca al servidor le pasamos las instancias que habia creado panel de control 
            self.server_hilo=threading.Thread(target=self.server.iniciar)
            self.server_hilo.deamon=True
            self.server_hilo.start()
            print("Servidor inciado ")
        else :
            print("El servidor ya esta en ejecucion")

    def detener_Servidor(self):
        if self.server is not None:
            self.server.detener()
            self.server=None
            
        else:
            print("El servidor no esta en ejecucion")

    def mostrar_menu(self):
        while True:
            print("\n--- Panel de Control del Servidor ---")
            print("1. Conectar Robot")
            print("2. Desconectar Robot")
            print("3. Activar Motores")
            print("4. Desactivar Motores")
            print("5. Cambiar Modo del Robot")
            print("6. Mover Robot")
            print("7. Activar Efector")
            print("8. Desactivar Efector")
            print("9. Hacer Homing")
            print("10. Iniciar Aprendizaje")
            print("11. Finalizar Aprendizaje")
            print("12. Mostrar Estado")
            print("13. Mostrar Log de Trabajo")
            print("14. Iniciar Servidor")
            print("15. Detener Servidor")
            print("16. Listar Comandos Disponibles")
            print("17. Salir")
            opcion = input("Seleccione una opción: ")

            if opcion == "1":
                print(self.robot.conectar())
            elif opcion == "2":
                print(self.robot.desconectar())
            elif opcion == "3":
                print(self.robot.activar_Motores())
            elif opcion == "4":
                print(self.robot.desactivar_Motores())
            elif opcion == "5":
                print(self.robot.cambiar_Modo())
            elif  opcion=="6":
                pos=input("Ingrese posicion (x,y,z): ")
                vel=input("Ingrese la velocidad:")
                print(self.robot.mover(pos,vel))

            elif  opcion=="7":
                print(self.robot.activar_Efector())
            elif  opcion=="8":
                print(self.robot.desactivar_Efector())
            elif  opcion=="9":
                print(self.robot.homing())
            elif opcion=="10":
                print(self.robot.iniciar_Aprendizaje())
            elif opcion=="11":
                print(self.robot.finalizar_Aprendizaje())
            elif  opcion=="12":
                estado = self.robot.mostrar_Estado()
                for clave, valor in estado.items():
                    print(f"{clave}: {valor}")

            elif  opcion=="13":
                print(self.robot.logger.mostrar_ultimas_lineas())
            
            elif opcion=="14":
                print(self.iniciar_Servidor())
            
            elif opcion=="15":
                print(self.detener_Servidor())
            
            elif opcion == "16":
                
                print("--- Lista de Comandos Disponibles ---")
                comandos=self.ayuda.listar_comandos()
                
                for comando,descripcion in comandos.items():
                    print(f"{comando}:{descripcion}")

            elif opcion == "17":
                print("Saliendo del panel de control...")
                break
            else:
                print("Opción no válida, intente nuevamente.")

# Servidor RPC completo
class ServidorRPCCompleto:
    
    def __init__(self, robot,logger,host='localhost', port=8080):
        
        self.server=SimpleXMLRPCServer((host,port),allow_none=True) # allow_none=True --> si tus métodos en el servidor pueden devolver o recibir valores None en las solicitudes y respuestas.
       
        self.robot=robot
        self.logger=logger

        self.server.register_function(self.probarConexion,'probarConexion')
        self.server.register_function(self.conectar, "conectar_Robot")
        self.server.register_function(self.desconectar, "desconectar_Robot")
        self.server.register_function(self.activar_Motores, "activar_Motores")
        self.server.register_function(self.desactivar_Motores, "desactivar_Motores")
        self.server.register_function(self.mover, "mover_Robot")
        self.server.register_function(self.activar_Efector, "activar_Efector")
        self.server.register_function(self.desactivar_Efector, "desactivar_Efector")
        self.server.register_function(self.homing, "homing")
        self.server.register_function(self.cambiar_modo, "cambiarModo")
        self.server.register_function(self.mostrar_estado, "mostrarEstado")
        self.server.register_function(self.iniciar_Aprendizaje, "iniciar_Aprendizaje")
        self.server.register_function(self.finalizar_Aprendizaje, "finalizar_Aprendizaje")

        
    def probarConexion(self):
        return 1
    
    def conectar(self):
        self.robot.conectar()
        return "Robot conectado"

    def desconectar(self):
        self.robot.desconectar()
        return "Robot desconectado"
    def activar_Motores(self):
        self.robot.activar_Motores()
        return "Motores activados"
    
    def desactivar_Motores(self):
        self.robot.desactivar_Motores()
        return "Motores desactivados"
    
    def mover(self,posicion,velocidad):
        self.robot.mover(posicion,velocidad)
        return "Robot movido con exito"
    
    def activar_Efector(self):
        self.robot.activar_Efector()
        return "Efector activado"

    def desactivar_Efector(self):
        self.robot.desactivar_Efector()
        return "Efector desactivado"
    
    def homing(self):
        self.robot.homing()
        return "Se ha llevado al robot al origen"

    def iniciar_Aprendizaje(self):
        self.robot.iniciar_Aprendizaje()
        return "Aprendizaje iniciado"
    def finalizar_Aprendizaje(self):
        self.robot.finalizar_Aprendizaje()
        return "Aprendizaje finalizado"

    def cambiar_modo(self):
        self.robot.cambiar_Modo()
        return f"Modo cambiado"

    def mostrar_estado(self):
        return self.robot.mostrar_Estado()

    def mostrar_Ayuda(self):
        ayuda=Ayuda()  #Intancia de ayuda ver si esta bien por qeu crearia una instancia cada vez que se usa el metodo
        return ayuda.listar_comandos()
    
    def iniciar(self):
        
        self.server.serve_forever()

    def detener(self):
        
        self.server.shutdown()
        print("Servidor RPC detenido")

# Iniciar el servidor RPC con el menú interactivo
if __name__ == "__main__":
    panel_control=PanelControl()
    panel_control.mostrar_menu()