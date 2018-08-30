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

Скомпилируйте REPL:
```
g++ -std=c++11 -Wall -Wextra -Werror -o repl repl.cpp Token.h Identifier.h ASTNode.h TokenTypes.h Lexer.cpp Lexer.h
Parser.cpp Parser.h Evaluator.cpp Evaluator.h TokenContainer.cpp TokenContainer.h SymbolTable.cpp SymbolTable.h
```

## Быстрый старт

Объявление переменной:
```
var a;
```

Назначение переменной:
```
var a
a = 5
```

Объявление и назначение переменной в одном выражении:
```
var a = 2.2;
var b = a;
```

Вычисление математических выражений:
```
2 + 5
(1 + 2) * -7.5
var a = 6 / 3
a + 3
```

### Подробное описание работы REPL'а
Более подробное описание вы можете найти в папке [docs](https://github.com/blinky-z/REPL/tree/master/docs/ru)