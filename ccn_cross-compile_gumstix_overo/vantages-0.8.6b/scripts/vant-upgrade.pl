#!/usr/bin/perl -w

use strict;

sub _usage
{
  print("vant-upgrade.pl [ <config file> ] | -h\n");
}

sub _update_db_schema
{
  my $p_sConfig = shift;

  warn("PUT WARNING HERE ABOUT USER ID AND STOPPING VANTAGED FIRST...");

  if (!defined($p_sConfig))
  {
    warn("Unable to update schema without config file.");
  }
  elsif (!open(CONF, "< $p_sConfig"))
  {
    warn("Unable to open config file: '$p_sConfig'");
  }
  else
  {
    my $sCxn = undef;
    my $sLine = undef;
    while ($sLine = <CONF>)
    {
      chomp($sLine);
      if ($sLine !~ /^\s*#/
          && $sLine =~ m/\s*db_cxn_string\s*=\s*(\S+)\s*$/)
      {
        $sCxn = $1;
        last;
      }
    }
    close(CONF);

    if (!defined($sCxn))
    {
      warn("Unable to find DB cxn string in config file '$p_sConfig'");
    }
    else
    {
      my $sResp = `echo 'select max(UPDATE) from PS_UPDATE_STATS;' | sqlite3 $sCxn`;
      chomp($sResp);

      my $iLastUpdate = 0;
      if ($sResp !~ /^\d+$/)
      {
        print("No updates found, starting from first.\n");
      }
      else
      {
        $iLastUpdate = $sResp;
        print("Starting after update $iLastUpdate\n");
      }

      my $sSqlSrc = "/usr/local/share/vantages/sql";
      if (!opendir(SQL_DDL, "$sSqlSrc/ddl"))
      {
        warn("Unable to open SQL DDL dir '$sSqlSrc/ddl'");
      }
      elsif (!opendir(SQL_DML, "$sSqlSrc/dml"))
      {
        warn("Unable to open SQL DML dir '$sSqlSrc/dml'");
        closedir(SQL_DDL);
      }
      else
      {
        my %hChunks;
        my $sFile = undef;
        while ($sFile = readdir(SQL_DDL))
        {
          if ($sFile =~ m/^(\d+)_ps_update\.sql$/)
          {
            my $iChunk = $1;
            if ($iChunk > $iLastUpdate)
            {
              $hChunks{$iChunk} = $iChunk;
            }
          }
        }
        closedir(SQL_DDL);

        while ($sFile = readdir(SQL_DML))
        {
          if ($sFile =~ m/^(\d+)_ps_update\.sql$/)
          {
            my $iChunk = $1;
            if ($iChunk > $iLastUpdate)
            {
              $hChunks{$iChunk} = $iChunk;
            }
          }
        }
        closedir(SQL_DML);

        foreach my $iChunk (sort(keys(%hChunks)))
        {
          if ( -e "$sSqlSrc/ddl/$iChunk" . "_ps_update.sql")
          {
            print("Upgrading DDL chunk $iChunk ('$sSqlSrc/ddl/$iChunk", "_ps_update.sql')\n");
            system("cat $sSqlSrc/ddl/$iChunk" . "_ps_update.sql | sqlite3 $sCxn");
          }
          if ( -e "$sSqlSrc/dml/$iChunk" . "_ps_update.sql")
          {
            print("Upgrading DML chunk $iChunk ('$sSqlSrc/dml/$iChunk", "_ps_update.sql')\n");
            system("cat $sSqlSrc/dml/$iChunk" . "_ps_update.sql | sqlite3 $sCxn");
          }
        }
      }
    }
  }
}

my $sConfig = shift;

if (!defined($sConfig))
{
  $sConfig = "/etc/vantaged.conf";
}

if ("-h" eq $sConfig)
{
  _usage();
}
else
{
  print("Updating DB schema...\n");
  _update_db_schema($sConfig);
  print("Done\n");
}
