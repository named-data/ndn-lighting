from pyccn import CCN, Name, Interest, ContentObject, Key, Closure

# configuration manager
# trusted authority that signs application namespaces for control

class test(Closure.Closure):
	def __init__(self):
		self.handle = CCN.CCN()
		self.name = Name.Name("ccnx:/ndn/ucla.edu/apps/lighting/TV1/fixture/")

		self.k = Key.Key()
		self.k.generateRSA(1024)

	# this is so we don't have to do signing each time someone requests data
	def prepareContent(self,name):
		co = ContentObject.ContentObject()

		# since they want us to use versions and segments append those to our name
		co.name = Name.Name(name) # making copy, so any changes to co.name won't change self.name
		co.name.appendVersion() # timestamp which is our version
		co.name += b'\x00' # first segment

		co.content = "Content Served - Test OK"

		si = ContentObject.SignedInfo()
		si.publisherPublicKeyDigest = self.k.publicKeyID
		si.type = 0x28463F # need to make the variable shorter
		si.finalBlockID = b'\x00' # no more segments available
		si.keyLocator = Key.KeyLocator(self.k)
		co.signedInfo = si

		co.sign(self.k)

		return co

	# Called when we receive interest
	# once data is sent signal ccn_run() to exit
	def upcall(self, kind, info):
		if kind != Closure.UPCALL_INTEREST:
			return Closure.RESULT_OK
		#print(info.Interest.name)
		#self.handle.put(self.content) # send the prepared data
		self.handle.put(self.prepareContent(info.Interest.name)) # send the dynamic data (to match interest name)
		#self.handle.setRunTimeout(-1) # finish run()
		print("published content object at "+str(info.Interest.name))
		return Closure.RESULT_INTEREST_CONSUMED

	def start(self):
		
		#self.content = self.prepareContent(self.name)

		# register our name, so upcall is called when interest arrives
		self.handle.setInterestFilter(self.name, self)


		print "\nlistening at "+str(self.name)
		
		# express interests for own keys, so they're netcached

		# enter ccn loop (upcalls won't be called without it, get
		# doesn't require it as well)
		# -1 means wait forever
		self.handle.run(-1)



Test = test()
Test.start()


