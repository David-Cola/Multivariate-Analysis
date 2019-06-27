# Multivariate-Analysis

  The purpose of the Multivarite Analysis is to optimize the cuts on the mass of a mother particle. In this case our mother particle will be a Bottom Meson denoted * B_MM. A cut is to set a restriction on particle variables. For example ///IPCH2_OWNPV can select a distance from the particle decaying to the primary vertex. If we know what the distance of the particle is supposed to be then we can confidently filter out most  of the readings that detector picks up as background. Each reading is known as an event, and we can filter out the background events. Unfortunatley we will always have to sacrifice some signal, so how do we know how much signal to background we should take out? We can use a fit function built by Henry which in this case  is labeled ///Fit2MeV2.C. The fit gives you multiple parameters that we have set, but we're mostly interested in parameter 0 and parameter 2. Parameter 0 being the total events in the signal region and parameter 2 being sigma.It was decided that our figure of merit (labeled "fom" in the code) was
  
 fom = Parameter[0] / ((Total Entries - Parameter [0])^(1/2) - (Parameter[2] /2)) 
 
 Going further it's important to note some cuts concadinate with eachother i.e. * B_IPCHI2_OWNPV and p1_MC15TuneV1_ProbNNp. How do we know what cuts are best? We could go through every 20 possible cuts on IPCHI2_OWNPV and use them on all 30 possible cuts on ProbNNp which would give us 600 different 
     
