def testmultiline():
    data = [
        1, 2, 3,
        4, 5, 6
    ] #  }{итрый комментарий
    # Просто комментарий

    total = sum(data)
    
    print("Больше 10") # Это комментарий
    print("Меньше или равно 10")
        
    call_unexisting_function1(total) # И это тоже комментарий
    call_unexisting_function2(total)
    call_unexisting_function3(total)

    assert total == 6
