use std::fs::*;
use std::io::prelude::*;

pub fn parse(path: String) {
    
    let mut file = File::open(path);

    if file.is_err() {
        return;
    }

    let mut contents = String::new();
    file.unwrap().read_to_string(&mut contents);

    print!("{}", contents);
        

        

}