import scala.actors.Futures._

package test {


object Pi {

  def main(args: Array[String]) = {
    val f = future { (1 to 1000000000).foldLeft(0)(_+_) }
    println(f().toString);
    println("coucou");
  }

}


} // package
