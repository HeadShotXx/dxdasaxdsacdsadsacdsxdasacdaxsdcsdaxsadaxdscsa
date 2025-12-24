use anti_dump::obfuscate;

// This function would typically be in the application logic, not in the anti-dump library
fn deobfuscate(data: &[u8]) -> String {
    const KEY: u8 = 0xAB; // The same key used for obfuscation
    let mut deobfuscated = Vec::with_capacity(data.len());
    for &byte in data.iter() {
        deobfuscated.push(byte ^ KEY);
    }
    String::from_utf8(deobfuscated).unwrap_or_else(|_| "Invalid UTF-8".to_string())
}

fn main() {
    let obfuscated_string = obfuscate!("Hello, world!");
    println!("Obfuscated: {:?}", obfuscated_string);

    let deobfuscated_string = deobfuscate(&obfuscated_string);
    println!("Deobfuscated: {}", deobfuscated_string);
}
