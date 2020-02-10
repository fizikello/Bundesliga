#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <regex>
#include <algorithm>

using namespace std;

const string teamNames[18] = {"FC Bayern Muenchen",  "Werder Bremen", "Eintracht Frankfurt", "Schalke 04", "FC Augsburg", "VfL Wolfsburg",
            "Hamburger SV", "FC Ingolstadt", "1. FC Koeln", "SV Darmstadt", "Borussia Dortmund", "FSV Mainz 05",
            "Borussia Moenchengladbach", "Bayer Leverkusen", "Hertha BSC Berlin", "SC Freiburg", "TSG 1899 Hoffenheim", "RasenBall Leipzig"};

class Result{

  private:
    string home;
    string away;
    int    home_goals;
    int    away_goals;
    
  public:
    Result(string & h, string & a, int & hg, int & ag) : home(h), away(a), home_goals(hg), away_goals(ag)
    {};
    ~Result(){};
    Result(){};
    
    string getHome(){ return home; };
    string getAway(){ return away; };
    int getHomeGoals(){ return home_goals; };
    int getAwayGoals(){ return away_goals; };
};

class Team
{
  private:
    string name;
    int  played_matches;
    int  won_matches;
    int  tie_matches;
    int  lost_matches;
    int  points;
    int  scored;
    int  gotten;
  public:
  Team(string & n) : name(n), points(0), scored(0), gotten(0), played_matches(0),
                       won_matches(0), tie_matches(0), lost_matches(0)
    {};
    ~Team(){};
    string getName()   const   { return name; };
    int    getScored() const  { return scored ;};
    int    getGotten() const  { return gotten ;};
    int    getPoints() const  { return points ;};
    int    getPlayedMatches() const { return played_matches;} ;
    int    getWonMatches()    const { return won_matches;} ;
    int    getTieMatches()    const { return tie_matches;} ;
    int    getLostMatches()   const { return lost_matches;} ;
    
    bool operator <(const Team& t) const 
    {
      int points = this->points - t.points;
      int diff   = this->scored - this->gotten - t.scored + t.gotten;
      int moreG  = this->scored - t.scored;
      
      if( points != 0 )
      {
        return this->points > t.points;
      }
      else if( diff != 0 )
      {
       return (this->scored - this->gotten) > (t.scored - t.gotten);
      }
      else if( moreG != 0 )
      {
       return this->scored > t.scored;
      }
      else
      {
        string a, b;
        a = this->name;
        b = t.name;
        transform(a.begin(), a.end(), a.begin(), ::tolower);
        transform(b.begin(), b.end(), b.begin(), ::tolower);
        
        return a < b;
      }
      
    }
    
    string toString()
    { 
        string s = string(30, ' ');
        s.replace(s.begin(), s.begin() + name.length(), name);
        
        s +=  to_string(played_matches) + "  ";
        s +=  to_string(won_matches) + "  ";
        s +=  to_string(tie_matches) + "  ";
        s +=  to_string(lost_matches) + "  ";
        s +=  to_string(scored) + ":";
        s +=  to_string(gotten) + "  ";
        s +=  to_string(points);
        
        return s;
    } 
    
    void   setName(string & v){name = v;};
    void   setScored( int &v){ scored += v; };
    void   setGotten( int &v){ gotten += v; };
    void   setPoints( int v){ points += v;};
    void   setPlayedMatches(){ ++played_matches; };
    void   setWonMatches(){ ++won_matches; };
    void   setTieMatches(){ ++tie_matches; };
    void   setLostMatches(){ ++lost_matches; };
};

class Match
{  
  public:
  
    void setPoints(int s, int g, Team & t)
    {
      t.setPlayedMatches();
      t.setScored(s);
      t.setGotten(g);
      
      if( s-g > 0 )
      {
        t.setWonMatches();
        t.setPoints(3);
      }
      else if( s == g )
      {
        t.setTieMatches();
        t.setPoints(1);
      }
      else
      {
        t.setLostMatches();
        t.setPoints(0);
      }
    }
    
    Result makeResult(string & s)
    {    
    regex pattern01( "(\\d+):(\\d+)\\s(.*)\\s-\\s(.*)" );
    smatch w;
    
    string home = "";
    string away = "";
    int    homeGoals = 0;
    int    awayGoals = 0;
    
    if( regex_search( s, w, pattern01 ) ){
    homeGoals = atoi(string(w[1]).c_str());
    awayGoals = atoi(string(w[2]).c_str());
    home = string(w[3]);
    away = string(w[4]);}
    
    return Result( home, away, homeGoals, awayGoals );
    };
    
    void update( Result & r, vector<Team> & t )
    {      
      for(int i = 0; i < t.size(); i++)
      {
        if( !r.getHome().compare(t.at(i).getName()) )
        { 
          setPoints(r.getHomeGoals(), r.getAwayGoals(), t.at(i));
        }
        else if( !r.getAway().compare(t.at(i).getName()) )
        {
          setPoints(r.getAwayGoals(), r.getHomeGoals(), t.at(i));
        }
      }
    };
};

class Bundesliga
{
private:
    vector<Team> teams;
    bool comparator(const Team& t1, const Team& t2);
    vector<string> setRanking( vector<Team> & t );
    
public:
    string table(vector<string> results)
    {
        string s = "";
        
        for(int i = 0; i < 18; i++)
        {    
          string tmp = teamNames[i];
          Team t(tmp);
          teams.push_back(t);
        }
        
        Match m;
        
        for( int i = 0; i < results.size(); i++ )
        {
          Result r = m.makeResult(results.at(i));
          if( r.getHome() != "" )
            m.update(r, teams);
        }    
        
        sortTeams(teams);
        vector<string> ranking = setRanking(teams);
            
        for(int i = 0; i < teams.size(); i++)
        {
          s += ranking.at(i) + teams.at(i).toString() + "\n";
        }
         
         s=s.substr(0, s.length()-1);       
        return s;
    }
    
     bool getMatchResult(vector<string> & results);
     bool sortTeams(vector<Team> & t);
};

bool Bundesliga::sortTeams( vector<Team> & t)
{
  sort( t.begin(), t.end() );
  return true;
}

bool Bundesliga::comparator(const Team& t1, const Team& t2)
{
  return t1.getPoints() > t1.getPoints();
}

vector<string> Bundesliga::setRanking( vector<Team> & t )
{
  vector<int> rank(t.size(), 0);
  vector<string> out(t.size(), "  . ");
  
  rank.at(0) = 1;
  
  for(int i = 1; i < t.size(); i++)
  {
     if( t.at(i).getPoints() == t.at(i-1).getPoints() && (t.at(i).getScored() - t.at(i).getGotten()) == (t.at(i-1).getScored() - t.at(i-1).getGotten()) && t.at(i).getScored() == t.at(i-1).getScored() )
     {
       rank.at(i) = rank.at(i-1);
     } 
     else
       rank.at(i) = i + 1;
  }
  
  for( int i = 0; i < rank.size(); i++ )
  {     
        if( rank.at(i) > 9)
          out.at(i).replace(out.at(i).begin(), out.at(i).begin() + to_string(rank.at(i)).length(), to_string(rank.at(i)));
        else
          out.at(i).replace(out.at(i).begin()+1, out.at(i).begin()+2, to_string(rank.at(i)));
  }
  return out;
}
