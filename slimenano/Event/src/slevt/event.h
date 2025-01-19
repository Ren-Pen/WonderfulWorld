#pragma once
#ifndef FILE_SLIMENANO_EVENT_EVENT_H
#    define FILE_SLIMENANO_EVENT_EVENT_H
#    include <cstdint>
#    include <slevt.h>

namespace slimenano::event {

    using EventIdentifier = uint64_t;

    class SLIMENANO_API Event {
      public:
        explicit Event(const EventIdentifier id): m_id(id) {}
        virtual ~Event() = default;

        [[nodiscard]] EventIdentifier id() const { return m_id; }
      protected:

      private:
        EventIdentifier m_id;
    };
} // namespace slimenano::event

#endif // FILE_SLIMENANO_EVENT_EVENT_H
