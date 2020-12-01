<h1 align="center"> EventManager </h1><br>
Tiny single-header C++ Event System.
EventManager focuses on easy to use syntax, without the need to subclass 'Event' types.


## Registering Function Callbacks
```cpp
#include "EventManager.h"

const int key_w = 87;

struct KeyPressEvent
{
  int key_code;
}

void on_key_press(KeyPressEvent* event)
{
    // ...
}

int main()
{
    EventManager::listen<KeyPressEvent>(&on_key_press);
    
    EventManager::fire<KeyPressEvent>(key_w);
}
```

## Registering Member Function Callbacks
```cpp
#include "EventManager.h"

const int key_w = 87;

struct KeyPressEvent
{
  int key_code;
}

class InputHandler
{
public:
    InputHandler()
    {
        EventManager::listen<KeyPressEvent>(this, &InputHandler::on_key_press);
    }
private:
    void on_key_press(KeyPressEvent* event)
    {
         // ...
    }
}

int main()
{
    InputHandler input_handler;
    
    EventManager::fire<KeyPressEvent>(key_w);
}
```

