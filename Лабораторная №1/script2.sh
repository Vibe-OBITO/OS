#!/home/noname/021731_Аблакулов_Амаль
# первый аргумент ($1) - имя файла для записи результата
# второй ($2) - директория, в которой ищем файлы
# третий ($3) - расширение искомых файлов (в формате txt, т.е. без точки)
if [[ ! -d "/home/noname/021731_Аблакулов_Амаль" ]]; then
    echo "Директория "/home/noname/021731_Аблакулов_Амаль" не найдена"
else
    find "/home/noname/021731_Аблакулов_Амаль" -type f -name "*.txt" -fprint "file"
fi
exit 0
