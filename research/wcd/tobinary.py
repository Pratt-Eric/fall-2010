import sys
def tobinary(filename):
    ''' This program transforms the profile output to binary format. '''
    pfile = open(filename, 'r')
    DEBUG_FORMAT1 = False
    DEBUG_FORMAT = False
    for line in pfile: 
        items = line.split('\t')

        # test the format is corrent. 
        result = ''

        for item in items:
            if item.strip() == '': continue
            # find profile setting items.
            if item.find('basic_info:') >= 0:
                # sex
                if item.find('Sex') >= 0: result += '1 '
                else: result += '0 '
                # networks
                if item.find('Networks') >= 0: result += '1 '
                else: result += '0 '
                # relationship status e.g. married. 
                if item.find('Relationship Status') >= 0: result += '1 '
                else: result += '0 '
                # interested in e.g. women. 
                if item.find('Interested In') >= 0: result += '1 '
                else: result += '0 '
                # biobliography bio. 
                if item.find('About ') >= 0: result += '1 '
                else: result += '0 '

            if item.find('philosophy:') >= 0: 
                if item.find('Favorite Quotations') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Religious Views') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Political Views') >= 0: result += '1 '
                else: result += '0 '

            if item.find('friends:') >= 0: 
                if item.find('YES') >= 0: result += '1 '
                else: result += '0 '

            if item.find('photos:') >= 0: 
                if item.find('YES') >= 0: result += '1 '
                else: result += '0 '
                
            if item.find('wallpost:') >= 0: 
                if item.find('YES') >= 0: result += '1 '
                else: result += '0 '

            if item.find('contact:') >= 0:
                if item.find('Website') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Address') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Screen Name') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Email') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Facebook') >= 0: result += '1 '
                else: result += '0 '
                if item.find('Phone') >= 0: result += '1 '
                else: result += '0 '

            if item.find('PROFILE:') >= 0:
                # Married to
                if item.find('Married to') >= 0: result += '1 '
                else: result += '0 '
                # DOB
                if item.find('Born on') >= 0: result += '1 '
                else: result += '0 '
                # current location. 
                if item.find('Lives in') >= 0: result += '1 '
                else: result += '0 '
                # home town
                if item.find('From ') >= 0: result += '1 '
                else: result += '0 '

            if item.find('act_interests:') >= 0:
                if item.find('Activities') >= 0: result += '1 ' 
                else: result += '0 '
                if item.find('Interests') >= 0: result += '1 '
                else: result += '0 ' 

            if item.find('edu_work') >= 0:
                if item.find('Employers') >= 0: result += '1 ' 
                else: result += '0 '
                if item.find('Grad School') >= 0: result += '1 '
                else: result += '0 ' 
                if item.find('College') >= 0: result += '1 ' 
                else: result += '0 '
                if item.find('High School') >= 0: result += '1 '
                else: result += '0 ' 

        print result

if __name__ == "__main__":
    tobinary(sys.argv[1])
