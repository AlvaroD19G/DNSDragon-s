1. Información del Proyecto
Nombre del Proyecto: Sensor de Riesgo Preventivo DNSDragons
Equipo: 
•	Kristy Alvarado Gutiérrez
•	David Duarte García
•	Andrés Gutiérrez Herrera
•	Gerald Alfaro Solórzano
Roles:
•	Ensamblaje: Kristy Alvarado Gutiérrez, David Duarte García, Andrés Gutiérrez Herrera y Gerald Alfaro Solórzano.
•	Página WEB: Kristy Alvarado Gutiérrez, David Duarte García, Andrés Gutiérrez Herrera y Gerald Alfaro Solórzano.
•	Documentación: Kristy Alvarado Gutiérrez, David Duarte García, Andrés Gutiérrez Herrera y Gerald Alfaro Solórzano.
________________________________________
2. Descripción y Justificación
Problema que se aborda: Crear un interfaz físico digital que permita interacción manual con un potenciómetro y 3 luces LED de distintos colores, para avisar de manera preventiva a los conductores de maniobras bruscas o de riesgos al conducir para asegurar la integridad de los pasajeros y del conductor.
Importancia y contexto: La razón del desarrollo de este proyecto nace por el riesgo vial al cual se exponen las personas que tanto manejan buses como los que viajan en ellos, buscando la manera de alertar a los conductores que la manera en la cual están conduciendo puede afectar la integridad tanto del conductor como de los pasajeros.
Usuarios/beneficiarios: Personas que utilizan los servicios de transporte público y trabajadores que se dedican al manejo de buses.
________________________________________
3. Objetivos del Proyecto
Objetivo General:
Desarrollar un sistema embarcado para autobuses que prevenga el exceso de velocidad en zonas críticas y detecte maniobras bruscas, emitiendo alertas sonoras y visuales al conductor y registrando eventos para análisis de la empresa de buses.
Objetivos Específicos:
1.	Simular velocidad con el potenciómetro y determinar estado de riesgo.
2.	Usar tres LED para codificar el riesgo: Verde (Seguro), Azul (Precaución), Rojo (Peligro).
3.	Implementar patrones de buzzer: sin sonido (Seguro), beep intermitente (Precaución) y tono continuo (Peligro).
4.	Definir límites por zona.
5.	Detectar conducción brusca.
Implementaciones a futuro: Integrar GPS para coordenadas, integrar IMU (acelerómetro/giroscopio) para giros con mayor precisión y enviar telemetría a un backend y generar reportes por unidad/ruta.
________________________________________
4. Requisitos Iniciales
Lista breve de lo que el sistema debe lograr:
Requisito 1- Generar por medio de luces LED una alerta preventiva de incremento de velocidad o riesgo.
Requisito 2- Generar por medio de un buzzer un sonido intermitente que varía dependiendo del riesgo al cual se expone el bus.
Requisito 3- Mostrar en la página web el tipo de riesgo el cual se está registrando en el microcontrolador.
________________________________________
6. Diseño Preliminar del Sistema Diseño Preliminar del Sistema
Arquitectura inicial (diagrama):
Componentes previstos:
•	Microcontrolador: ESP32.
•	Sensores/actuadores: Potenciómetro, Buzzer, LED rojo + resistencia 220 Ω, LED verde + resistencia 220 Ω, LED azul + resistencia 220 Ω, jumpers y protoboard.
•	Página Web: Web desarrollada con Arduino IDE con javascript, css y html.
Esquemas:
Conexiones de ESP32-Sensores-Protoboard
•	Buzzer: GND, 3.3v -> IO32
•	Potenciómetro: GND, 3.3v -> IO34
•	LED Verde: IO25 -> I15-I16 -> J16 - Ω
•	LED Azul: IO26 -> I9-I10 -> J10 - Ω
•	LED Roja: IO27 -> I3-I4 -> J4 - Ω
________________________________________
6. Plan de Trabajo
Cronograma preliminar
Hito	Descripción	Fecha
H1	Ensamblaje del microcontrolador con sus adiciones 	19-08-2025
H2	Configuración del ESP32 por medio de Arduino IDE	19-08-2025
H3	Creación de la página web	19-08-2025
H4	Pruebas y documentación 	19-08-2025
		
Riesgos identificados y mitigaciones:
•	Que algún led se queme por el uso constante de los LED, ya que los LED que vayan a ser usados deben ser de buena calidad.
•	La sensibilidad del potenciómetro, ya que cuesta calibrar un punto intermedio del sensor para que identifique correctamente la velocidad del potenciómetro.
•	La integridad del buzzer, puede llegar a fallar el buzzer generando que las luces LED se enciendan, pero no se emita sonido.
________________________________________
7. Prototipos conceptuales (si aplica)
Código mínimo de prueba:
(Ver carpeta de código de prueba)
Evidencia visual sugerida:
(Ver carpeta de evidencia de imágenes)
Siguientes pasos (escala):
•	Integrar GPS y IMU (acelerómetro/giroscopio).
•	Backend para telemetría y tablero de control.
