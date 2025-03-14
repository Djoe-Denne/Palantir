#ifndef ICONTENT_SIZE_OBSERVER_HPP
#define ICONTENT_SIZE_OBSERVER_HPP

#include "core_export.hpp"

namespace palantir::window::component {

/**
 * @class IContentSizeObserver
 * @brief Interface for objects that need to observe content size changes
 * 
 * This interface defines the contract for objects that want to be notified
 * when the size of content changes, such as a window that needs to adjust
 * its size to match the content.
 */
class PALANTIR_CORE_API IContentSizeObserver {
public:
    IContentSizeObserver(const IContentSizeObserver&) = delete;
    IContentSizeObserver(IContentSizeObserver&&) = delete;
    auto operator=(const IContentSizeObserver&) -> IContentSizeObserver& = delete;
    auto operator=(IContentSizeObserver&&) -> IContentSizeObserver& = delete;
    virtual ~IContentSizeObserver() = default;

    /**
     * @brief Notification of content size change
     * 
     * @param width The new content width
     * @param height The new content height
     */
    virtual void onContentSizeChanged(int width, int height) = 0;

protected:
    IContentSizeObserver() = default;
};

} // namespace palantir::window::component

#endif // ICONTENT_SIZE_OBSERVER_HPP 