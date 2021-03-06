#!/bin/bash
# Переименование всех файлов с указанным суффиксом путем добавления
# к ним заданного префикса.
 if [ $# -eq 0 ]; then
  echo "Вы не ввели ни суффикс, ни префикс. Введите: bash $0 суффикс префикс"
   elif [ $# -eq 1 -a "$1" != "?" ]; then
    echo "Вы ввели только суффикс и не ввели префикс. Введите: bash $0 суффикс префикс"
     elif [ $# -eq 2 ]; then
       echo "Переименование файлов с указанным суффиксом. Продолжить? [y/n]"
       while true ; do
         read -p "" yn
         case $yn in
           [Yy]* ) break;;
           [Nn]* ) exit;;
           * ) echo "Введите 'y' для того чтобы продолжить и 'n' для того чтобы выйти.";;
         esac
       done
 elif [ $# -gt 2 ]; then
  echo "Неправильное количество аргументов. Введите: bash $0 суффикс префикс"
  exit
 fi
if [ "$1" = "?" ]; then
 echo "Добавляет префикс ко всем файлам в папке, с указанным суффиксом."
 echo "Формат ввода: bash $0 суффикс префикс"
fi
counter=0
if [ $# -gt 1 ]; then
for loop in *"$1"
do
  counter=`expr $counter + 1 `
  mv "$loop" "$2$loop"
done
 case $counter in
 0)
  echo "Файлов с заданным суффиксом нет"
  ;;
 *)
  echo "Файлы переименованы успешно"
  ;;
 esac
fi