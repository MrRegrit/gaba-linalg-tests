# gaba-linalg-tests

Набор модульных тестов для библиотеки линейной алгебры [gaba-linalg](gaba-linalg/README.md). Использует GoogleTest (gtest) и исполняемую цель `MatrixTests`.

## Состав

Проект объединяет две части:

- `gaba-linalg` — подпроект-библиотека `LinAlgLib`, которая тестируется (подключается через `add_subdirectory`).
- `source/tests.cpp` — все тесты на GoogleTest для класса `linalg::Matrix`.

GoogleTest подтягивается автоматически через `FetchContent` из указанного в `CMakeLists.txt` архива, поэтому отдельная установка gtest не требуется.

## Требования

- Компилятор с поддержкой C++17;
- CMake 3.10 или новее;
- доступ в сеть при первой настройке (скачивание GoogleTest).

## Сборка и запуск тестов

```bash
mkdir build && cd build
cmake ..
cmake --build .
./Debug/MatrixTests.exe
```

## Покрываемые сценарии

Файл `source/tests.cpp` содержит тесты, сгруппированные по фикстурам:

- `MatrixState` — начальное состояние, размеры и ёмкость, обход через `begin`/`end`;
- `MatrixConstructors` — все конструкторы, копирующий и перемещающий присваивания, списки инициализации;
- `MatrixModification` — `reshape`, `reserve`, `clear`, `shrink_to_fit`, `swap`, доступ и проверка границ;
- `MatrixArithmetic` — унарные `+`/`-`, сложение, вычитание, умножение на скаляр и матриц, сравнение, несовместимые размеры;
- `MatrixOutput` — форматированный вывод;
- `MatrixComputations` — `norm`, `trace`, `det`, `rank`, `concatenate`, `transpose`, `invert`, `power`, `solve` и их свойства;
- `MatrixAccess` — доступ через `operator[]`/`RowProxy`, включая выход за границы;
- `MatrixExtraMath` — `minor`, `cofactor`, `adjugate`, `sum`, `min`, `max`, `dot`;
- `MatrixProperties` — `is_square`, `is_symmetric`, `is_identity`, `is_orthogonal`;
- `MatrixFactory` — `zeros` и `identity`.

Дополнительно проверяется, что вычислительные методы (`det`, `rank`, `invert`, `solve`) не изменяют исходные матрицы.

## Структура проекта

```
gaba-linalg-tests
├── CMakeLists.txt        # цель MatrixTests, подключение LinAlgLib и GoogleTest
├── source/tests.cpp      # исходный код всех тестов
└── gaba-linalg/          # тестируемая библиотека (см. её собственный README)
```
