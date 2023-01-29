#!/bin/bash
number=5
while [ $number -gt 0 ] # условие - пока значение number greater-then(-gt) 0
do
echo "Значение = $number" # выводим значение
number=$[ $number - 1 ] # уменьшаем number
done