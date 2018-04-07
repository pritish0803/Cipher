#wrls-aodv.tcls
# Define options
set val(chan)   Channel/WirelessChannel ;# channel type
set val(prop)   Propagation/TwoRayGround ;# radio-propagation model
set val(netif)  Phy/WirelessPhy ;# network interface type



set val(ant)    Antenna/OmniAntenna ;# antenna model
set val(ifqlen) 50  ;# max packet in ifq
set val(nn)     3  ;# number of mobilenodes
set val(rp)     AODV ;# routing protocol AODV or DSR

set val(x)      500 ;# X dimension of topography
set val(y)      400 ;# Y dimension of topography
set val(mac)    Mac/802_11 ;# MAC type
set val(ifq)    Queue/DropTail/PriQueue ;# interface queue type
set val(ll)     LL ;# link layer type








set val(stop) 70 ;# time of simulation end
set ns [new Simulator]
set tracefd [open tracef.tr w]
set windowVsTime2 [open win.tr w]
set namtrace [open simwrls.nam w]
$ns trace-all $tracefd
$ns namtrace-all-wireless $namtrace $val(x) $val(y)
# set up topography object
set topo [new Topography]
$topo load_flatgrid $val(x) $val(y)
create-god $val(nn)
# Create nn mobilenodes [$val(nn)] and attach them to the channel. # configure the nodes
$ns node-config -adhocRouting $val(rp) \
  -llType $val(ll) \
  -macType $val(mac) \
  -ifqType $val(ifq) \
  -ifqLen $val(ifqlen) \
  -antType $val(ant) \
  -propType $val(prop) \
  -phyType $val(netif) \
  -channelType $val(chan) \
  -topoInstance $topo \
  -agentTrace ON \
  -routerTrace ON \
  -macTrace OFF \
  -movementTrace ON
for {set i 0} {$i < $val(nn) } { incr i } {
set node_($i) [$ns node] 
$node_($i) color black
}

$ns color 0 Red

# set shape and label

$node_(0) label "SENDER" 


$node_(1) label "Receiver" 


$node_(2) label "LOSS in Packets."


 # Provide initial location of mobilenodes
$node_(0) set X_ 5.0 
$node_(0) set Y_ 5.0 
$node_(0) set Z_ 0.0

$node_(1) set X_ 150.0 
$node_(1) set Y_ 150.0 
$node_(1) set Z_ 300.0

$node_(2) set X_ 300.0 
$node_(2) set Y_ 5.0 
$node_(2) set Z_ 0.0


for {set i 0} {$i < 2} {incr i} {
$ns at 0.1 "$node_($i) color blue"
}
$ns at 0.2 "$node_(2) color yellow"


# Set a TCP connection between node_(0) and node_(1)
set tcp [new Agent/TCP/Newreno]
$tcp set class_ 2
set sink [new Agent/TCPSink]
$ns attach-agent $node_(0) $tcp
$ns attach-agent $node_(1) $sink
$ns connect $tcp $sink
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ns at 10.0 "$ftp start"

# Define node initial position in nam
for {set i 0} {$i < $val(nn)} { incr i } {
# 30 defines the node size for nam
$ns initial_node_pos $node_($i) 30
}

# Telling nodes when the simulation ends
for {set i 0} {$i < $val(nn) } { incr i } {
$ns at $val(stop) "$node_($i) reset"; }
# ending nam and the simulation
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "stop"
$ns at 80.0 "puts \"end simulation\";$ns halt"



proc attach-expoo-traffic { node sink size burst idle rate } {
#Get an instance of the simulator
set ns [Simulator instance]
#Create a UDP agent and attach it to the node
set source [new Agent/UDP]
$ns attach-agent $node $source
#Create an Expoo traffic agent and set its configuration parameters
set traffic [new Application/Traffic/Exponential]
$traffic set packetSize_ $size
$traffic set burst_time_ $burst
$traffic set idle_time_ $idle
$traffic set rate_ $rate
# Attach traffic source to the traffic generator
$traffic attach-agent $source
#Connect the source and the sink
$ns connect $source $sink
return $traffic
}
set sink0 [new Agent/LossMonitor]

$ns attach-agent $node_(2) $sink0

set source0 [attach-expoo-traffic $node_(0) $sink0 200 2s 1s 10k]





set f0 [open out0.tr w]

proc stop {} {
global ns tracefd namtrace f0
$ns flush-trace
close $tracefd
close $namtrace
close $f0

#Call xgraph to display the results
exec xgraph out0.tr  &
exec nam simwrls.nam &
exit 0 }
#------------------------------------------------------------------------------------------------------- #Now we can write the procedure which actually writes the data to the output files.
 proc record {} {
global sink0  f0
#Get an instance of the simulator
set ns [Simulator instance]
#Set the time after which the procedure should be called again
set time 1.0
#How many bytes have been received by the traffic sinks?
set bw0 [$sink0 set bytes_]

#Get the current time
set now [$ns now]
#Calculate the bandwidth (in MBit/s) and write it to the files
puts $f0 "$now [expr $bw0/$time*8/1000000]"

#Reset the bytes_ values on the traffic sinks
$sink0 set bytes_ 0

#Re-schedule the procedure
$ns at [expr $now+$time] "record"
}


proc parameter {n0 n1} {
global ns x1 x2 y1 y2 z1 z2 d
set x1 [expr int([$n0 set X_])] 
set y1 [expr int([$n0 set Y_])]
set z1 [expr int([$n0 set Z_])]

set x2 [expr int([$n1 set X_])]
set y2 [expr int([$n1 set Y_])]
set z2 [expr int([$n1 set Z_])]

set d [expr int(sqrt(pow(($x2-$x1),2)+pow(($y2-$y1),2)+pow(($z2-$z1),2)))]
puts "Distance between node node_0 and node_1 = $d"

set Propagation_time [expr $d/200000000]
puts "Propagation Time = $Propagation_time"
}

$tcp set fid_ 0


$ns at 0.0 "record"

$ns at 10.0 "$source0 start"

$ns at 50.0 "$source0 stop"

#$ns at 55.0 "parameter $node_(0) $node_(1)"
$ns at 60.0 "stop"

$ns run
