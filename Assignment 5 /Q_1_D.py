import requests
import subprocess
import re
usrname='afridi'
psd='pak12345istan'


def getchecksum(filename):
    s = subprocess.check_output(["python","hashextend.py","-if","Q_1_D.py"])
    print("this is checksum"+str(s))
    #chksm=s.decode("utf-8").split(" ")[0]
    #print(s.decode("utf-8").split(" ")[0])
    start=s.find("start:",)+7
    end=s.find("end",start)
    gar=s[start:end]
    return re.sub(r"\W","",gar)
    


def login_user(session):
    url='https://emsec.cispa.saarland/media/download/'
    data = { 'username': usrname, 'pwd':psd,'app':getchecksum('Q_1_D.py')}
    response = session.post(url, data)
    success = response.status_code < 400
    response_text = response.text
    #print(response_text)

    if "Username or Password wrong. Does this user exist" in response_text:
        print("********************************************************************************\n")
        print("Username or Password wrong. Does this user exist :) Try with correct credentials")
        print("\n********************************************************************************\n")

    if "This is not a registered app" in response_text:
        print("*************************************************************\n")
        print("Error Your application is not a trusted application")
        print("\n*************************************************************\n")
    else:
        with open('content.png','wb') as out:
            out.write(response.content)

    print("done")
    return response_text, success

if __name__ == '__main__':
	user_session = requests.session()
	login_user(user_session)

