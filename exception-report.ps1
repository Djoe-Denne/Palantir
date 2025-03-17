$directory = Get-Location  # Change this to your project root if needed
$fileTypes = @("*.hpp", "*.cpp")

# Recursively search for all relevant files
$files = Get-ChildItem -Path $directory -Recurse -Include $fileTypes

foreach ($file in $files) {
    $lines = Get-Content $file.FullName
    for ($i = 0; $i -lt $lines.Count; $i++) {
        if ($lines[$i] -match "throw std::runtime_error") {
            $startLine = [math]::Max(0, $i - 5)
            $endLine = $i

            Write-Output "* File: $($file.FullName)"
            Write-Output "** Line Number Start: $($startLine + 1)"
            for ($j = $startLine; $j -le $endLine; $j++) {
                Write-Output "*** $($lines[$j])"
            }
            Write-Output "** Line Number End: $($endLine + 1)"
            Write-Output "----"
        }
    }
}
