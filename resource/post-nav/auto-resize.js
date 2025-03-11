(function() {
    previousHeight = 0;
    previousWidth = 0;
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
            window.chrome.webview.postMessage({
                type: 'contentSize',
                width: width,
                height: height
            });
        }
    }
    
    // Report size immediately after load
    reportContentSize();
    
    // Set up a MutationObserver to detect DOM changes
    const observer = new MutationObserver(function(mutations) {
        // Wait a bit for any animations or transitions to complete
        setTimeout(reportContentSize, 500);
    });
    
    // Start observing the document with configured parameters
    observer.observe(document, {
        attributes: true,
        childList: true,
        subtree: true,
        characterData: true
    });
    
})(); 