# LavadoraArduino
Cabe aclarar que yo estudio ingeniería en sistemas y, aunque sí logro dominar C++, todavía van a encontrar una que otra falta o mala práctica. Me falta experiencia usando Arduino y programando, ya que no es parte de mi carrera usar Arduino. Menciono esto para que aquellos que tengan tiempo me ayuden a mejorarlo. También, para evitar algún comentario negativo, me gustaría aprender más. Saludos a quien lea esto y suerte.

# ¿Que puede hacer?
Actualmente creo ya esta implementada toda la funcionalidad de una lavadora lo unico falta afinar es lo del nivel del agua
# Datos importantes
El archivo "webpageCode.h" contiene el HTML de la página. La verdad es que todavía no sé exactamente cuáles son las limitaciones del ESP32 en cuanto a lo compleja que puede ser la página, pero bueno, para implementar algún cambio de diseño, está el archivo "index.html", que es la misma página pero para que la modifiques. Para agregar la nueva modificación, simplemente debes copiar y pegar dentro de const char webpageCode[] = R"=====( *pegue aquí su código HTML, CSS, JS *)=====";

Tambien quiero aclarar que las funciones y tiempos de mi codigo son para mi lavadora pero lo hice lo mas dinamico posible para que se adapte a cualquier lavadora con un minimo de cambios, nota* la funcion tambien del llenado se puede controlar con el presostato de la lavadora pero la mia no le sirve asi que tendran que implementar

