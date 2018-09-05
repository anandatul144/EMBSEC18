import requests
import subprocess
import re
import sys

usrname='afridi'
psd='pak12345istan'

# this function calls the c program using
# subprocess check_output calling the function to measure
# the file itself passing the name of the client software
def getchecksum(filename):

    s = subprocess.check_output(["./hashextend5","-if",filename])
    #print("this is checksum"+str(s))
    #chksm=s.decode("utf-8").split(" ")[0]
    #print(s.decode("utf-8").split(" ")[0])

    # just use this technique to capture the
    # hash from the hashextend6.c program that
    # we are calling to measure
    start=s.find("length 32",)+9
    end=s.find("end",start)
    gar=s[start:end]
    print(s)
    # finaly reuturn the hash as simple
    return re.sub(r"\W","",gar)
    


# this function create session
# specifiy the endpoint url
#check the reponse if it is valid
# print the secret content else print the error
# Q_1_D.py is the client program
def login_user(session):
    url='https://emsec.cispa.saarland/media/download/'
    data = { 'username': usrname, 'pwd':psd,'app':getchecksum('Q_1_D.py')}
    response = session.post(url, data)
    success = response.status_code < 400
    response_text = response.text

    # print the error if the credentials are not correct and exit
    if "Username or Password wrong. Does this user exist" in response_text:
        print("********************************************************************************\n")
        print("Username or Password wrong. Does this user exist :) Try with correct credentials")
        print("\n********************************************************************************\n")
        sys.exit(-1)
    # print the error if the application is not trusted print the error  message and exit
    if "This is not a registered app" in response_text:
        print("*************************************************************\n")
        print("Error Your application is not a trusted application")
        print("\n*************************************************************\n")
    else:
        #if everything goes will the credentials are correct and the application is trusted
        # then will sace the content as it is png image we write and save it content.png
        with open('content.png','wb') as out:
            out.write(response.content)
        print("*************************************************************\n")
        print("Well done the apllication is trusted and now you have the secret content")
        print("\n*************************************************************\n")
    # just return :)
    return response_text, success

if __name__ == '__main__':
	user_session = requests.session()
	login_user(user_session)

