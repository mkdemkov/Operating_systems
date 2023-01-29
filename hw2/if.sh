#!/bin/bash
if pwd # в терминале вводится команда pwd
then # если выполнение успешное, выводится соотв. сообщение
echo "Succesfully worked!"
else # если что-то пошло не так, даем пользователю знать об этом
echo "Something went wrong..."
fi