# C style guide

This style guide is to help us write clean, consistent code in ENCE361. This style-guide should apply to all of the code we write, but not to any library code we downloaded. For example, this guide does not apply to ```buttons.h``` and ```buttons.c``` because this code is a library written by the teaching team for interacting with buttons, while this style guide does apply to ```task_button_polling.h``` and ```task_button_polling.c``` as we wrote that module.

## Order in source file.c

The following headings show how to order the different types of code in a source file.

#### 1. module docstring

Re-state the module name (from filename), and describe the module purpose.

#### 2. include corrisponding header file

For example, ```task_scheduler.c``` would include ```task_scheduler.h```.
Use include with quotes:
```c
#include "task_scheduler.h"
```

#### 3. include other modules we created

For example, ```app.c``` may include ```task_scheduler.h```.
Use include with quotes:
```c
#include "task_scheduler.h"
```

#### 4. include other non-builtin libraries

For example, ```task_button_polling.c``` may include ```stm32_hal.h```
Use include without quotes:
```c
#include <task_scheduler.h>
```

#### 5. include standard libraries (stdlib.h)

Use include without quotes:
```c
#include <stdlib.h>
```

#### 6. defines

defines should be all caps:
```c
#define VALUE 33
```

#### 7. global static variables

```c
static uint32_t num_of_steps = 44;
```

This also includes enum and structure definitions.

#### 8. static function forward declarations

```c
static uint32_t get_num_steps(void);
```

#### 9. fully-written public functions

```c
uint32_t PEDOMETER_get_steps(void)
{
    return num_of_steps;
}
```

#### 10. fully-written static functions

```c
static uint32_t get_num_steps(void)
{
    return num_of_steps;
{
```

### Line Spacing

- 1 newline after module docstring
- 0 newlines between related includes (in same heading above)
- 1 newline between non-related includes (in different headings above)
- 2 newlines between includes and defines
- 0 newline between defines.
- 2 newlines between defines and global static variables.
- 0 newlines between global static variables.
- 2 newlines between global static variables and static function forward declarations.
- 0 newlines between static functions.
- 2 newlines between static functions and fully-written public functions
- 1 newline between each fully-written public function
- 1 newline between fully-written public functions and fully-written static functions
- 1 newline between fully-written static functions

## Order in header file.h

The following headings show how to order the different types of code in a header file.

#### 1. module docstring

Must be same as corrisponding source file.c

#### 2. header guard begin

```c
#ifndef PEDOMETER_H
#define PEDOMETER_H
```

#### 3. defined types

```c
typedef struct {
    uint32_t value_one;
    uint32_t value_two;
} JOYSTICK_State;
```

#### 4. includes

See above information about how to structure includes.

#### 5. public functions

```c
void PEDOMETER_get_steps(void);
```

#### 6. header guard end

```c
#endif  /* INC_PEDOMETER_H_ */
```

### Line Spacing


- 1 newline between module docstring and header guard begin
- 2 newlines between header guard begin and includes.
- 2 newlines between includes and public functions
- 0 newlines between each public function
- 2 newline between public functions and header guard end

## General Style Rules

### Spelling

Use American english for all spellings.

### Function formatting

All opening and closing curly braces (```{``` and ```}```) should be on their own line.

### Indentation

Always use 4-space indentation for everything. No tabs!

### Function naming

Public functions should start with the module name, such as:
```c
uint32_t PEDOMETER_get_steps(void);
```

Private/static functions should not start with the module name, such as:
```c
uint32_t meters_to_kilometers(uint32_t meters)
```

### Forward-declaration for functions

All static functions should have a forward declaration at the top of the source file, and there implementation below the public functions in that file.

Forward declarations for public functions should be in the header file only.

Forward declarations should include parameter names, for example:
```c
uint32_t calculate_percentage(int32_t joystick_raw_value);
```
instead of:
```c
uint32_t calculate_percentage(int32_t);
```

### variable naming

Variable names should be words separated by underscores (NOT CamelCase!). For example, use ```num_of_steps```, and don't use ```numOfSteps```.

Special variable names that are not words:

- ```i``` to represent index
- ```j``` to represent inner-loop index, for nested loops
- ```c``` to represent character

### ```if```, ```for```, and ```while``` formatting.

Always use a space before opening parenthesis, and make opening curly braces on the same line:
```c
if (value) {
    // code
}

for (uint32_t i=0; i<10; i++) {
    // code
}

while (1) {
    // code
}
```

If using an ```else```, put it on the same line as the opening ```if```:
```c
if (value) {
    // code
} else {
    // code
}
```

If using if-(else-if)-else logic, place the ```if``` on the same line after the else:
```c
if (value_one) {
    // code
} else if (value_two) {
    // code
} else {
    // code
}
```

### breaking expressions into multiple lines

Break expressions with the operator on the next line, and always have brackets around them:
```c
uint32_t value = ((value_one + value_two) / value_three
                  + value_four + value_five);
```

Do the same for an ```if``` statement:
```c
if (state1->percent_x == state2->percent_x
    && state1->percent_y == state2->percent_y
    && state1->is_up == state2->is_up
    && state1->is_right == state2->is_right) {
    // code
}
```


### Pointer declarations

Always make the pointer character (```*```) be close to the type. For example, do it like this:
```
uint32_t* value;
```
Not this:
```
uint32_t *value;
```


### Function Pointers

Always use address-of operator (```&```) when dealing with function pointers. For example:

```c
uint32_t(*func_ptr)(void);

func_ptr = &my_func;
```

Do not do this:
```c
uint32_t(*func_ptr)(void);

func_ptr = my_func;
```
