public class Switch {
public String tableSwitch(int value) {
  return switch (value) {
    case 1 -> "one";
    case 2 -> "two";
    case 4 -> "four";
    default -> "other";
  };
}
}