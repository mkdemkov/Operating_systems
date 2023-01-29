#!/bin/bash
# объявляем функцию
function iamfunction { 
    echo "You have called the function"
}
number=3
while [ $number -gt 0 ] # цикл, который проверяет, что значение number больше нуля
do
iamfunction # вызов функции
number=$[ $number - 1 ] # изменение number
done
echo "Script has ended"