#pragma once

class NonCopyable
{
  protected:
    // Class NonCopyable has protected constructor and destructor members
    // to emphasize that it is to be used only as a base class.
    NonCopyable() {}
    ~NonCopyable() {}

  private:
    // Prevent NonCopyable class and any class inheriting it from being
    // copied or constructed by copy.
    NonCopyable(const NonCopyable&);
    NonCopyable& operator=(const NonCopyable&);
};
