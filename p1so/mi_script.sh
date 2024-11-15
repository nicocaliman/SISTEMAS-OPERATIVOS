#!/bin/bash

# Leer el archivo /etc/passwd línea por línea
while IFS=':' read -r user passwd uid gid gecos home shell; do
    # Comprobar si el home es un subdirectorio de /home
    if [[ "$(dirname "$home")" == "/home" ]]; then
        # Mostrar la entrada en el formato deseado
        echo -e "Usuario: $user\nContraseña: $passwd\nUID: $uid\nGID: $gid\nGecos: $gecos\nHome: $home\nShell: $shell\n"
    fi
done < /etc/passwd
