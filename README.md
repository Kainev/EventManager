# EventManager
Tiny single-header C++ Event System.
EventManager focuses on easy to use syntax, without the need to subclass 'Event' types.

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
