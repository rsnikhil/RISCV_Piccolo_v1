#!/usr/bin/perl -w
#==========================================================================
# filters_gendata.pl
#
# Author : Christopher Batten (cbatten@mit.edu)
# Date   : May 9, 2005
#
(our $usageMsg = <<'ENDMSG') =~ s/^\#//gm;
#
# Simple script which creates an input data set and the reference data 
# for the image filters benchmark.
#
ENDMSG

use strict "vars";
use warnings;
no  warnings("once");
use Getopt::Long;

#--------------------------------------------------------------------------
# Command line processing
#--------------------------------------------------------------------------

our %opts;

sub usage()
{

  print "\n";
  print " Usage: filters_gendata.pl [options] \n";
  print "\n";
  print " Options:\n";
  print "  --help  print this message\n";
  print "  --size  size of input pixmap [32]\n";
  print "  --seed  random seed [555]\n";
  print "$usageMsg";

  exit();
}

sub processCommandLine()
{

  $opts{"help"} = 0;
  $opts{"size"} = 32;
  $opts{"seed"} = 555;
  Getopt::Long::GetOptions( \%opts, 'help|?', 'size:i', 'seed:i' ) or usage();
  $opts{"help"} and usage();

}

#--------------------------------------------------------------------------
# Helper Functions
#--------------------------------------------------------------------------

sub print2DArray
{
  my $arrayName = $_[0];
  my $arrayRef  = $_[1];

  my $arrayLen = scalar(@{$arrayRef});

  print "int ".$arrayName."[DATA_SIZE][DATA_SIZE] = \n";
  print "{\n";
  for ( my $i = 0; $i < $arrayLen; $i++ ) {
    print "  ";
    printInlineArray($arrayRef->[$i]);
    if ( $i != $arrayLen-1 ) {
      print ",\n";
    }
  }
  print "\n";

  print  "};\n\n";
}

sub printInlineArray
{
  my $arrayRef  = $_[0];

  my $arrayLen = scalar(@{$arrayRef});

  print "{";
  for ( my $i = 0; $i < $arrayLen; $i++ ) {
    print sprintf("%3d", $arrayRef->[$i]);
    if ( $i != $arrayLen-1 ) {
      print ", ";
    }
  }
  print  "}";
}



#--------------------------------------------------------------------------
# Main
#--------------------------------------------------------------------------

sub main()
{

  processCommandLine();
  srand($opts{"seed"});
  
  #For now we'll store RGB as separate matrixes
  
  #Create a blank pixmap
  my @pixmap_r;
  my @pixmap_g;
  my @pixmap_b;
  for ( my $i = 0; $i < $opts{"size"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"size"}; $j++ ) {
	$pixmap_r[$i][$j] = 0;
	$pixmap_g[$i][$j] = 0;
	$pixmap_b[$i][$j] = 0;
    }
  }
  
  # Add a number of randomly colored rectangles
  
  my $numrects = int($opts{"size"});
  
  for (my $n = 0; $n < $numrects; $n++)
  {
    my $rectred = int(rand(255));
    my $rectgreen = int(rand(255));
    my $rectblue = int(rand(255));
    my $rectwidth = int(rand($opts{"size"} / 3)) + 1;
    my $rectheight = int(rand($opts{"size"} / 3)) + 1;
    my $rectx = int(rand($opts{"size"} - $rectwidth));
    my $recty = int(rand($opts{"size"} - $rectheight));
    print sprintf("// Adding <%d, %d, %d> rectangle with dimensions (%d x %d) at (%d, %d).\n", $rectred, $rectgreen, $rectblue, $rectwidth, $rectheight, $rectx, $recty);
    for (my $i = $recty; $i < ($recty + $rectheight); $i++) 
    {
      for (my $j = $rectx; $j < ($rectx + $rectwidth); $j++)
      {
	$pixmap_r[$i][$j] = $rectred;
	$pixmap_g[$i][$j] = $rectgreen;
	$pixmap_b[$i][$j] = $rectblue;
      }
    }
  }
  
  # An extremely simple filter just to get started
  
  my @removered_r;
  my @removered_g;
  my @removered_b;
  
  for ( my $i = 0; $i < $opts{"size"}; $i++ ) {
    for ( my $j = 0; $j < $opts{"size"}; $j++ ) {
	$removered_r[$i][$j] = 0;
	$removered_g[$i][$j] = $pixmap_g[$i][$j];
	$removered_b[$i][$j] = $pixmap_b[$i][$j];
    }
  }

  # A sharpen filter
  
  my @sharpen_r;
  my @sharpen_g;
  my @sharpen_b;
  my @sharpen_filter = ( ['0','-1','0'], ['-1','5','-1'],['0','-1','0'] );
  
  my $sum_r = 0;
  my $sum_g = 0;
  my $sum_b = 0;
  
  for ( my $i = 0; $i < $opts{"size"}; $i++ ) 
  {
    $sharpen_r[0][$i] = $pixmap_r[0][$i]; 
    $sharpen_g[0][$i] = $pixmap_g[0][$i]; 
    $sharpen_b[0][$i] = $pixmap_b[0][$i]; 
    $sharpen_r[$opts{"size"} - 1][$i] = $pixmap_r[$opts{"size"} - 1][$i]; 
    $sharpen_g[$opts{"size"} - 1][$i] = $pixmap_g[$opts{"size"} - 1][$i]; 
    $sharpen_b[$opts{"size"} - 1][$i] = $pixmap_b[$opts{"size"} - 1][$i]; 
    
    $sharpen_r[$i][0] = $pixmap_r[$i][0]; 
    $sharpen_g[$i][0] = $pixmap_g[$i][0]; 
    $sharpen_b[$i][0] = $pixmap_b[$i][0]; 
    $sharpen_r[$i][$opts{"size"} - 1] = $pixmap_r[$i][$opts{"size"} - 1]; 
    $sharpen_g[$i][$opts{"size"} - 1] = $pixmap_g[$i][$opts{"size"} - 1]; 
    $sharpen_b[$i][$opts{"size"} - 1] = $pixmap_b[$i][$opts{"size"} - 1]; 
    
  }
  
  for ( my $i = 1; $i < ($opts{"size"} - 1); $i++ ) 
  {
    for ( my $j = 1; $j < ($opts{"size"} - 1); $j++ ) 
    {
      $sum_r = 0;
      $sum_g = 0; 
      $sum_b = 0; 
      
      for ( my $k = 0; $k < 3; $k++ ) 
      {
	for ( my $l = 0; $l < 3; $l++ ) 
	{
	  my $x = $i + $k - 1;
	  my $y = $j + $l - 1;
	  
	  $sum_r += $pixmap_r[$x][$y] * $sharpen_filter[$k][$l];
	  $sum_g += $pixmap_g[$x][$y] * $sharpen_filter[$k][$l];
	  $sum_b += $pixmap_b[$x][$y] * $sharpen_filter[$k][$l];
	}
      }
      
      $sharpen_r[$i][$j] = ($sum_r > 255)? 255 : ($sum_r >= 0) ? $sum_r : 0;
      $sharpen_g[$i][$j] = ($sum_g > 255)? 255 : ($sum_g >= 0) ? $sum_g : 0;
      $sharpen_b[$i][$j] = ($sum_b > 255)? 255 : ($sum_b >= 0) ? $sum_b : 0;
      
    }
  }
  
  print "\n\#define DATA_SIZE ".$opts{"size"}." \n\n";
  print2DArray("input_data_r", \@pixmap_r);
  print2DArray("input_data_g", \@pixmap_g);
  print2DArray("input_data_b", \@pixmap_b);
  print2DArray("verify_data_r", \@sharpen_r);
  print2DArray("verify_data_g", \@sharpen_g);
  print2DArray("verify_data_b", \@sharpen_b);

}

main();

