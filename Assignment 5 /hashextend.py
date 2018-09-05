import argparse
import sys
import os.path
import subprocess
import os
import re
def get_chunks(file_size):
    chunk_start = 0
    chunk_size = 1024  # 131072 bytes, default max ssl buffer size
    while chunk_start + chunk_size < file_size:
        yield(chunk_start, chunk_size)
        chunk_start += chunk_size

    final_chunk_size = file_size - chunk_start
    yield(chunk_start, final_chunk_size)


def read_file_chunked(file_path):
    handle=subprocess.check_output(["./hashsequencestart"])
    #print("I am sparta"+str(handle))
    hand=str(handle).split(" ")[2][0:8]
    #print("\n"+hand+"\n")
    

    with open(file_path,"rb") as file_:
        file_size = os.path.getsize(file_path)

        #print('\nFile size: {}\n'.format(file_size))

        progress = 0

        for chunk_start, chunk_size in get_chunks(file_size):

            file_chunk = file_.read(chunk_size)

            with open ('file.img','wb') as f:
                f.write(file_chunk)
            f.close()
            # do something with the chunk, encrypt it, write to another file...
            #print(file_chunk)
            seq=subprocess.call(["./sequenceupdate",'-hs',hand,'-if','file.img',])
            #print("hahahahahah"+str(seq))
            #os.remove('file.img')
            progress += len(file_chunk)
            #print('{0} of {1} bytes read ({2}%)'.format(
            #    progress, file_size, int(progress / file_size * 100))
            #)

    complete=subprocess.check_output(["./sequencecomplete",'-hs',hand ,'-v','-of','hash.bin']) 
        
    start=complete.find("Result length 32")+17
    end=complete.find("sequencecomplete: success",start)
    
    gar=complete[start:end]
    return re.sub(r"\W", "",gar)
    

def check_file(filename):
    if not os.path.exists(filename):
        raise argparse.ArgumentTypeError('argument filename does not exists')
    return filename


def check_arg(args=None):
    parser = argparse.ArgumentParser(description='Script for question 1 b')
    parser.add_argument('-if','--InputFile',help='Input File Name',required='True',type=check_file)

    results = parser.parse_args(args)
    return (results.InputFile)




if __name__ == '__main__':
    filename=check_arg(sys.argv[1:])
    #print(filename)
    out=read_file_chunked(filename)
    print("\nstart:\n"+out+"\n:end")
    #return(out)

