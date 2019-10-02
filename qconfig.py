# Before you can use the jobs API, you need to set up an access token.
# Log in to the IBM Q experience. Under "Account", generate a personal 
# access token. Replace 'PUT_YOUR_API_TOKEN_HERE' below with the quoted
# token string.

APItoken = 'PUT_YOUR_API_TOKEN_HERE'


if 'APItoken' not in locals():
    raise Exception('Please set up your access token. See Qconfig.py.')
