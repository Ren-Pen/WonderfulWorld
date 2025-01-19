#pragma once
#ifndef __FILE_SLIMENANO_EVENT_EVENT_H
#    define __FILE_SLIMENANO_EVENT_EVENT_H
#    include <cstdint>
#    include <slevt.h>

namespace slimenano::event {

    using EventIdentifier = uint64_t;

    class SLIMENANO_API Event {
      public:
        Event(EventIdentifier id): m_id(id) {};
        virtual ~Event() = default;

        EventIdentifier id() const { return m_id; }
      private:
        EventIdentifier m_id;
    };
} // namespace slimenano::event

#endif // __FILE_SLIMENANO_EVENT_EVENT_H
