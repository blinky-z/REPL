# REPL

## Вступление
Простая интерактивная среда программирования, запускаемая из консоли. Поддерживаются только математические выражения, а также работа
с переменными в рамках математических выражений.

## Установка

Клонируйте данный репозиторий с **GitHub**:

```
mkdir ~/REPL
cd ~/REPL
git clone https://github.com/blinky-z/REPL.git .
```

## Быстрый старт

Объявление и назначение переменной:
```
var a
var b = 6
a = 4
```

Вычисление математических выражений:
```
2 + 5
(1 + 2) * -7.5
var a = 6 / 3
a + 3
```

Булевы выражения:
```
var a = 5 < 10
a == true
var b = (10 == 10) && true
```

Функции:
```
func int add(var int a, var int b) {
    return a + b
}

var a = add(5, 6)
a // 11
```

```
func bool isGreater(var int l, var int r) {
    return l > r
}

isGreater(5, 0) // true
```

```
func bool boolOr(var bool a, var bool b) {
    return a || b
}

boolOr(true, false) // true
```

Параметры передаются в функции по значению:
```
func void assignLocal(var int a, var int newVal) {
    a = newVal
}

var a = 8
assign(a, 5)
a // 8
```

Видимость функций - глобальная, поэтому они имеют доступ к переменным вне параметров (если вы компилируете программу, сначала требуется объявить переменную):
```
func void assignGlobalA() {
    a = 5
}

var a
assignGlobalA()
print(a) // 5
```

Built-in функции:
```
print(5) // 5
```

### Подробное описание работы REPL'а
Более подробное описание вы можете найти в папке [docs](https://github.com/blinky-z/REPL/tree/master/docs/ru)