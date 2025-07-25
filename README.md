# Proyecto de Sistemas Operativos – 2024
Simulación y sincronización de procesos e hilos en C

Este proyecto académico tiene como objetivo experimentar con conceptos clave de sistemas operativos, tales como la planificación de procesos, sincronización de hilos, comunicación entre procesos y gestión de memoria. 
Se implementaron distintas soluciones en lenguaje C utilizando pipes, colas de mensajes, semáforos e hilos, aplicando técnicas de programación concurrente y modelos clásicos.

**🧪 Componentes principales:** 

  - Pumper Nic (Procesos + Pipes): Simulación de una cadena de comida rápida con atención diferenciada a clientes VIP, uso de procesos e hilos para la atención, y comunicación por pipes y colas de mensajes.

  - Mini Shell (System calls): Construcción de una shell simplificada que permite crear/eliminar archivos y directorios, listar contenido, mostrar contenido de archivos y modificar permisos, todo sin usar system(), solo llamadas al sistema.

  - Taller de Motos (Hilos + Semáforos): Modelado de ensamblado secuencial de motos con múltiples operarios. Se implementó sincronización estricta por turnos utilizando semáforos, garantizando exclusión mutua y orden correcto de las etapas.

  - Santa Claus Problem: Resolución del clásico problema de sincronización con múltiples hilos y condiciones de prioridad entre elfos y renos, priorizando eficiencia en el uso de semáforos.

**🔧 Tecnologías y herramientas:**
  - Lenguaje C

  - Hilos (pthreads)

  - Semáforos POSIX

  - Pipes y colas de mensajes

  - Makefile y scripts de ejecución

**Autor**
**Brenda Martinez**
