# A comment
# Another one

local func fn;
fn;

func main() -> u8 {
    fn = main;
    var a = 1;
    b = 1++; #should throw an error bc b is undefined
    
    return 0;
}