# A comment
# Another one

{
    func fn;
}
func fn
{
    func fn;
}

# solid func fn
# solid func fn;
#fn;

func main() -> u8 {
    fn = main;
    var a = 1;
    b = 1++; #should throw an error bc b is undefined
    
    return 0;
}