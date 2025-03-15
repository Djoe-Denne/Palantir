(function() {
    let previousHeight = 0;
    let previousWidth = 0;
    let resizeTimeout = null;
    
    // Function to measure and report content size
    function reportContentSize() {
        const width = Math.max(
            document.body.scrollWidth, 
            document.documentElement.scrollWidth,
            document.body.offsetWidth, 
            document.documentElement.offsetWidth,
            document.body.clientWidth, 
            document.documentElement.clientWidth
        );
        
        const height = Math.max(
            document.body.scrollHeight, 
            document.documentElement.scrollHeight,
            document.body.offsetHeight, 
            document.documentElement.offsetHeight,
            document.body.clientHeight, 
            document.documentElement.clientHeight
        );
        
        if (width !== previousWidth || height !== previousHeight) {
            previousWidth = width;
            previousHeight = height;
            
            // Clear any existing timeout
            if (resizeTimeout) {
                clearTimeout(resizeTimeout);
            }
            
            // Set a new timeout to send the message after 500ms
            resizeTimeout = setTimeout(() => {
                window.chrome.webview.postMessage({
                    type: 'contentSize',
                    event: {
                        width: width,
                        height: height
                    }
                });
                resizeTimeout = null;
            }, 50);
        }
    }
    
    // Report size immediately after load
    reportContentSize();
    
    // Set up a MutationObserver to detect DOM changes
    const observer = new MutationObserver(function(mutations) {
        // Call reportContentSize directly, it will handle the debouncing
        reportContentSize();
    });
    
    // Start observing the document with configured parameters
    observer.observe(document, {
        attributes: true,
        childList: true,
        subtree: true,
        characterData: true
    });
    
})(); 