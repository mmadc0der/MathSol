# Math Solver

Hey there. This is my study project. I want to create a simple interpreted programming language for solving simple math problems. And here we are.

## License

MIT

## Usage

For installation and use, you can select the version from ./release and download all the components presented.

The interpreter is used by running the executable file `mathsol.exe`.

Below is the output for `mathsol --help`:

```bash
MathSol v1.0.3 mathsol language interpreter
(C) 2024-now madc0der

Usage:
  mathsol [option] [file]

Options:
  -c, --command  : program passed in as a string
  -h, --help     : display this help information
  -V, --version  : display version information

Examples:
  mathsol                : Runs the interpreter interactively
  mathsol script.msol    : Executes code in script.msol file
  mathsol -c 1000 - 7    : Executes the expression after the -c argument
```

## Описание коммитов

| Название | Описание                                                        |
| -------- | --------------------------------------------------------------- |
| build    | Сборка проекта или изменения внешних зависимостей               |
| sec      | Безопасность, уязвимости                                        |
| ci       | Настройка CI и работа со скриптами                              |
| docs     | Обновление документации                                         |
| feat     | Добавление нового функционала                                   |
| fix      | Исправление ошибок                                              |
| perf     | Изменения направленные на улучшение производительности          |
| refactor | Правки кода без исправления ошибок или добавления новых функций |
| revert   | Откат на предыдущие коммиты                                     |
| style    | Правки по кодстайлу (табы, отступы, точки, запятые и т.д.)      |
| test     | Добавление тестов                                               |
