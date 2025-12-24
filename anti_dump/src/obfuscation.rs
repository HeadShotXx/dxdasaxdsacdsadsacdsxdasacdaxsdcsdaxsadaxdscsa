#[macro_export]
macro_rules! obfuscate {
    ($s:expr) => {{
        const KEY: u8 = 0xAB; // A simple XOR key, can be anything
        let s_bytes = $s.as_bytes();
        let mut result = Vec::with_capacity(s_bytes.len());
        for &byte in s_bytes.iter() {
            result.push(byte ^ KEY);
        }
        result
    }};
}
